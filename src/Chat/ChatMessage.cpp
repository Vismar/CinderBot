#include "ChatMessage.hpp"
#include <Utils/Config/ConfigurationManager.hpp>
#include <Utils/Config/ConfigurationParameters.hpp>
#include <QTime>

///////////////////////////////////////////////////////////////////////////

ChatMessage::ChatMessage()
{
    _author = "NoAuthor";
    _color = "Black";
    _message = "No message";
    _isModerator = false;
}

///////////////////////////////////////////////////////////////////////////

const QString& ChatMessage::GetAuthor() const
{
    return _author;
}

///////////////////////////////////////////////////////////////////////////

const QString& ChatMessage::GetColor() const
{
    return _color;
}

///////////////////////////////////////////////////////////////////////////

const QString& ChatMessage::GetTimeStamp() const
{
    return _timeStamp;
}

///////////////////////////////////////////////////////////////////////////

const QString& ChatMessage::GetMessage() const
{
    return _message;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::IsModerator() const
{
    return _isModerator;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::IsBroadcaster() const
{
    QString channelName;
    ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, channelName);
    return (channelName.toLower() == _author.toLower());
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetAuthor(const QString& author)
{
    _author = author;
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetColor(const QString& color)
{
    _color = "<font color=\"" + color + "\">";
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetMessage(const QString& message)
{
    _message = message;
    _SetTimeStamp();
}

///////////////////////////////////////////////////////////////////////////

MessageType ChatMessage::ParseRawMessage(const QString& message)
{
    MessageType msgType = Undefined;

    if (_IsNetworkMsg(message))
    {
        msgType = INFO;
        _author = SYSTEM_MESSAGE;
        _color = "<font color=\"Red\">";
        // Network messages comes only from bot itself, so just copy almost full message
        _message = message.mid(5);
    }
    else if (_IsPingCommand(message))
    {
        msgType = PING;
    }
    else if (_IsPongCommand(message))
    {
        msgType = PONG;
    }
    else
    {
        int ircCommand = _IsIrcComand(message);
        // If it is an IRC commands, check IRC code
        if (ircCommand != -1)
        {
            switch(ircCommand)
            {
            case 1:
                msgType = LOGIN_OK;
                break;
            default:
                break;
            }
        }
        else if (_IsConnectedToRoom(message))
        {
            // Notify about connection to the room
            msgType = INFO;
            _author = SYSTEM_MESSAGE;
            _color = "<font color=\"Red\">";
            _message = SYSTEM_MESSAGE_CR;
        }
        else if (_IsUserState(message))
        {
            msgType = USERSTATE;
        }
        else if (_IsJoinMsg(message))
        {
            msgType = JOIN;
            _GetAndSetAuthor(message);
        }
        else if (_IsPartMsg(message))
        {
            msgType = PART;
            _GetAndSetAuthor(message);
        }
        else if (_IsModeMessage(message))
        {
            msgType = MODE;
            _GetAndSetAuthorForMode(message, msgType);
        }
        else if (_IsUnmodeMessage(message))
        {
            msgType = UNMODE;
            _GetAndSetAuthorForMode(message, msgType);
        }
        else if (_IsPrivMsg(message))
        {
            msgType = PRIVMSG;
            _GetAndSetNameColor(message);
            _GetAndSetAuthor(message);
            _GetAndSetChatMessage(message);
            _GetAndSetModeratorFlag(message);
        }
    }
    // Set timestamp
    _SetTimeStamp();

    return msgType;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsNetworkMsg(const QString& message) const
{
    return message.startsWith("INFO");
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPingCommand(const QString& message) const
{
    return message.startsWith("PING");
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPongCommand(const QString& message) const
{
    return message.startsWith("PONG");
}

///////////////////////////////////////////////////////////////////////////

int ChatMessage::_IsIrcComand(const QString& message) const
{
    int result(-1);
    if (message.startsWith(":tmi.twitch.tv "))
    {
        result = message.mid(15, 3).toInt();
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsConnectedToRoom(const QString& message) const
{
    bool result(false);
    QString param;
    if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, param))
    {
        result = message.contains("JOIN") &&
                 !message.mid(1).contains(":") &&
                 message.contains(param);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsUserState(const QString& message) const
{
    return message.contains("USERSTATE");
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPrivMsg(const QString& message) const
{
    return message.startsWith("@badges=");
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsJoinMsg(const QString& message) const
{
    return message.contains("JOIN");
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPartMsg(const QString& message) const
{
    return message.contains("PART");
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsModeMessage(const QString& message) const
{
    return (message.contains("MODE") && message.contains("+o"));
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsUnmodeMessage(const QString& message) const
{
    return (message.contains("MODE") && message.contains("-o"));
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_SetTimeStamp()
{
    _timeStamp = QTime::currentTime().toString("h:mm:ss");
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetNameColor(const QString& message)
{
    // Default color
    QString color = "Black";

    int colorTagPosition = message.indexOf("color=");
    // 6 = length of "color="
    if (message.at(colorTagPosition + 6) != ';')
    {
        color = (message.mid(colorTagPosition + 6, 7));
    }
     _color = "<font color=\"" + color + "\">";
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetAuthor(const QString& message)
{
    // 13 = length of "display-name="
    int startOfTheName = message.indexOf("display-name=") + 13;
    int nameLength = message.indexOf("emotes") - startOfTheName - 1;
    if (nameLength < 1)
    {
        startOfTheName = message.indexOf("!") + 1;
        nameLength = message.indexOf("@", startOfTheName) - startOfTheName;
    }
    _author = message.mid(startOfTheName, nameLength);
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetAuthorForMode(const QString& message, MessageType msgType)
{
    QString modeType;
    if (msgType == MODE)
    {
        modeType = "+o";
    }
    else if (msgType == UNMODE)
    {
        modeType = "-o";
    }
    if (!modeType.isEmpty())
    {
        int startOfTheName = message.indexOf(modeType) + 3;
        if (startOfTheName >= 3)
        {
            int nameLength = message.length() - startOfTheName - 2; // "-2" for "\r\n"
            _author = message.mid(startOfTheName, nameLength);
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetChatMessage(const QString& message)
{
    QString name;
    ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, name);
    _message = message.right(message.length() - (message.indexOf("PRIVMSG #") + name.length() + 11));
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetModeratorFlag(const QString& message)
{
    QString name;
    ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, name);
    // If author is a broadcaster set mod flag true
    if (_author.toLower() == name)
    {
        _isModerator = true;
    }
    // If author not broadcaster, check status in message
    else
    {
        int index = message.indexOf("mod=");
        _isModerator = message.mid(index + 4,1).toInt();
    }
}

///////////////////////////////////////////////////////////////////////////
