#include "ChatMessage.hpp"
#include "../Utils/Config/ConfigurationManager.hpp"
#include "../Utils/Config/ConfigurationParameters.hpp"
#include <QTime>

bool ChatMessage::_IsNetworkMsg(const QString& message) const
{
    return message.startsWith("INFO");
}

bool ChatMessage::_IsPingCommand(const QString& message) const
{
    return message.startsWith("PING");
}

bool ChatMessage::_IsPongCommand(const QString& message) const
{
    return message.startsWith("PONG");
}

int ChatMessage::_IsIrcComand(const QString& message) const
{
    int result(-1);
    if (message.startsWith(":tmi.twitch.tv "))
    {
        result = message.mid(15, 3).toInt();
    }

    return result;
}

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

bool ChatMessage::_IsUserState(const QString& message) const
{
    return message.contains("USERSTATE");
}

bool ChatMessage::_IsPrivMsg(const QString& message) const
{
    return message.startsWith("@badges=");
}

void ChatMessage::_SetTimeStamp()
{
    _timeStamp = QTime::currentTime().toString("h:mm:ss");
}

const QString& ChatMessage::GetAuthor() const
{
    return _author;
}

const QString& ChatMessage::GetColor() const
{
    return _color;
}

const QString& ChatMessage::GetTimeStamp() const
{
    return _timeStamp;
}

const QString& ChatMessage::GetMessage() const
{
    return _message;
}

bool ChatMessage::IsModerator() const
{
    return _isModerator;
}

void ChatMessage::SetAuthor(const QString& author)
{
    _author = author;
}

void ChatMessage::SetColor(const QString& color)
{
    _color = "<font color=\"" + color + "\">";
}

void ChatMessage::SetMessage(const QString& message)
{
    _message = message;
    _SetTimeStamp();
}

MessageType ChatMessage::ParseRawMessage(const QString& message)
{
    MessageType msgType = Undefined;

    if (_IsNetworkMsg(message))
    {
        msgType = INFO;        
        _author = SYSTEM_MESSAGE;
        _color = "<font color=\"Red\">";
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
            msgType = INFO;
            _author = SYSTEM_MESSAGE;
            _color = "<font color=\"Red\">";
            _message = SYSTEM_MESSAGE_CR;
        }
        else if (_IsUserState(message))
        {
            msgType = USERSTATE;
        }
        else if (_IsPrivMsg(message))
        {
            msgType = PRIVMSG;
            QString color = "Black";
            int colorTagPosition = message.indexOf("color=");
            // 6 = length of "color="
            if (message.at(colorTagPosition + 6) != ';')
            {
                color = (message.mid(colorTagPosition + 6, 7));
            }
            // 13 = length of "display-name="
            int startOfTheName = message.indexOf("display-name=") + 13;
            int nameLength = message.indexOf("emotes") - startOfTheName - 1;
            if (nameLength < 1)
            {
                startOfTheName = message.indexOf("!") + 1;
                nameLength = message.indexOf("@", startOfTheName) - startOfTheName;
            }
            QString name(message.mid(startOfTheName, nameLength));
            _color = "<font color=\"" + color + "\">";
            _author = name;
            name.clear();
            ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, name);
            _message = message.right(message.length() -
                       (message.indexOf("PRIVMSG #") +
                       name.length() + 11));
            int index = message.indexOf("mod=");
            _isModerator = message.mid(index + 4,1).toInt();
        }
    }
    _SetTimeStamp();

    return msgType;
}
