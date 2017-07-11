/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ChatMessage.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include <QTime>
#include <QRegularExpressionMatch>

#define STR_PRIVMSG "PRIVMSG"
#define STR_WHISPER "WHISPER"

///////////////////////////////////////////////////////////////////////////
/************************** Regular expressions **************************/
///////////////////////////////////////////////////////////////////////////

/*** PING ***/
QRegularExpression ChatMessage::_regExpPing("PING :tmi.twitch.tv\\r\\n");
/*** PONG ***/
QRegularExpression ChatMessage::_regExpPong("PONG :tmi.twitch.tv\\r\\n");
/*** USERSTATE ***/
QRegularExpression ChatMessage::_regExpUserstate("@badges=.*;color=.*;display-name=.*;"
                                                 "emote-sets=.*;mod=.*;subscriber=.*;user-type=.* "
                                                 ":tmi.twitch.tv USERSTATE #.*\\r\\n");
/*** JOIN ***/
QRegularExpression ChatMessage::_regExpJoin(":.*!.*@.*.tmi.twitch.tv JOIN #.*\\r\\n");
/*** PART ***/
QRegularExpression ChatMessage::_regExpPart(":.*!.*@.*.tmi.twitch.tv PART #.*\\r\\n");
/*** MODE ***/
QRegularExpression ChatMessage::_regExpMode(":jtv MODE #.* \\+o .*\\r\\n");
/*** UNMODE ***/
QRegularExpression ChatMessage::_regExpUnmode(":jtv MODE #.* -o .*\\r\\n");
/*** PRIVMSG ***/
QRegularExpression ChatMessage::_regExpPrivmsg("@badges=.*;color=.*;display-name=.*;"
                                               "emotes=.*;id=.*;mod=.*;"
                                               "room-id=.*;subscriber=.*;tmi-sent-ts=.*;"
                                               "turbo=.*;user-id=.*;user-type=.* "
                                               ":.*!.*@.*.tmi.twitch.tv PRIVMSG #.* :.*\\r\\n");
/*** WHISPER ***/
QRegularExpression ChatMessage::_regExpWhisper("@badges=.*;color=.*;display-name=.*;"
                                               "emotes=.*;message-id=.*;thread-id=.*;"
                                               "turbo=.*;user-id=.*;user-type=.* "
                                               ":.*!.*@.*.tmi.twitch.tv WHISPER .* :.*\\r\\n");

///////////////////////////////////////////////////////////////////////////

ChatMessage::ChatMessage()
{
    _author = "NoAuthor";
    _realName = "NoName";
    _color = "Black";
    _timeStamp = "0:0:0";
    _message = "No message";
    _isModerator = false;
    _type = Undefined;
}

///////////////////////////////////////////////////////////////////////////
/***************************** Get functions *****************************/
///////////////////////////////////////////////////////////////////////////

const QString &ChatMessage::GetAuthor() const
{
    return _author;
}

///////////////////////////////////////////////////////////////////////////

const QString &ChatMessage::GetRealName() const
{
    return _realName;
}

///////////////////////////////////////////////////////////////////////////

const QString &ChatMessage::GetColor() const
{
    return _color;
}

///////////////////////////////////////////////////////////////////////////

const QString &ChatMessage::GetTimeStamp() const
{
    return _timeStamp;
}

///////////////////////////////////////////////////////////////////////////

const QString &ChatMessage::GetMessage() const
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

MessageType ChatMessage::GetType() const
{
    return _type;
}

///////////////////////////////////////////////////////////////////////////
/************************* Manual set functions **************************/
///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetAuthor(const QString &author)
{
    _author = author;
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetRealName(const QString &realName)
{
    _realName = realName;
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetColor(const QString &color)
{
    _color = "<font color=\"" + color + "\">";
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetMessage(const QString &message)
{
    _message = message;
    _SetTimeStamp();
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetModFlag(bool modFlag)
{
    _isModerator = modFlag;
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetType(MessageType type)
{
    _type = type;
}

///////////////////////////////////////////////////////////////////////////

MessageType ChatMessage::ParseRawMessage(const QString &message)
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
        else if (_IsWhisper(message))
        {
            msgType = WHISPER;
            _GetAndSetNameColor(message);
            _GetAndSetAuthor(message);
            _GetAndSetChatMessage(message);
            //_GetAndSetModeratorFlag();
        }
    }
    // Set timestamp
    _SetTimeStamp();
    _type = msgType;

    return msgType;
}

///////////////////////////////////////////////////////////////////////////
/**************************** Check functions ****************************/
///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsNetworkMsg(const QString &message) const
{
    return message.startsWith("INFO");
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPingCommand(const QString &message) const
{
    return _regExpPing.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPongCommand(const QString &message) const
{
    return _regExpPong.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////

int ChatMessage::_IsIrcComand(const QString &message) const
{
    int result(-1);
    if (message.startsWith(":tmi.twitch.tv "))
    {
        result = message.mid(15, 3).toInt();
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsConnectedToRoom(const QString &message) const
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

bool ChatMessage::_IsUserState(const QString &message) const
{
    return _regExpUserstate.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPrivMsg(const QString &message) const
{
    return _regExpPrivmsg.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsJoinMsg(const QString &message) const
{
    return _regExpJoin.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPartMsg(const QString &message) const
{
    return _regExpPart.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsModeMessage(const QString &message) const
{
    return _regExpMode.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsUnmodeMessage(const QString &message) const
{
    return _regExpUnmode.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsWhisper(const QString &message) const
{
    return _regExpWhisper.match(message).hasMatch();
}

///////////////////////////////////////////////////////////////////////////
/***************************** Set functions *****************************/
///////////////////////////////////////////////////////////////////////////

void ChatMessage::_SetTimeStamp()
{
    _timeStamp = QTime::currentTime().toString("h:mm:ss");
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetNameColor(const QString &message)
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

void ChatMessage::_GetAndSetAuthor(const QString &message)
{
    // Get real name
    int startOfTheName = message.indexOf("!") + 1;
    int nameLength = message.indexOf("@", startOfTheName) - startOfTheName;
    _realName = message.mid(startOfTheName, nameLength);
    // Get display name
    // 13 = length of "display-name="
    startOfTheName = message.indexOf("display-name=") + 13;
    nameLength = message.indexOf("emote") - startOfTheName - 1;
    // If display name was not specified, use real name
    if (nameLength < 1)
    {
        _author = _realName;
    }
    else
    {
        _author = message.mid(startOfTheName, nameLength);
    }
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetAuthorForMode(const QString &message, MessageType msgType)
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
            _realName = _author;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetChatMessage(const QString &message)
{
    QString name;
    ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, name);
    QString keyWord = STR_PRIVMSG;
    keyWord += " #";
    int startOfMsg = message.indexOf(keyWord);
    if (startOfMsg == -1)
    {
        keyWord = STR_WHISPER;
        keyWord += " ";
        startOfMsg = message.indexOf(keyWord);
        ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, name);
    }
    startOfMsg += name.length() + keyWord.length() + 2; // 2 = length of " :"
    _message = message.mid(startOfMsg, message.size() - startOfMsg - 2);
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::_GetAndSetModeratorFlag(const QString &message)
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
