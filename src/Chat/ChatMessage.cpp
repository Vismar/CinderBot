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

using namespace Chat;

///////////////////////////////////////////////////////////////////////////
/************************** Regular expressions **************************/
///////////////////////////////////////////////////////////////////////////

/*** PING ***/
QRegularExpression ChatMessage::_regExpPing("PING :tmi.twitch.tv\\r\\n");
/*** PONG ***/
QRegularExpression ChatMessage::_regExpPong("PONG :tmi.twitch.tv\\r\\n");
/*** USERSTATE ***/
QRegularExpression ChatMessage::_regExpUserstate("@badges=.*;color=(?<color>#\\w\\w\\w\\w\\w\\w);display-name=(?<name>.*);"
                                                 "emote-sets=.*;mod=(?<mod>\\d);subscriber=.*;user-type=.* "
                                                 ":tmi.twitch.tv USERSTATE #.*\\r\\n");
/*** ROOMSTATE ***/
QRegularExpression ChatMessage::_regExpRoomState("@broadcaster-lang=(?<lang>.*);emote-only=(?<emote>\\d);"
                                                 "followers-only=(?<followers>.*);r9k=(?<r9k>\\d);"
                                                 "room-id=.*;slow=(?<slow>.*);subs-only=(?<subs>\\d) "
                                                 ":tmi.twitch.tv ROOMSTATE #.*\r\n");
/*** JOIN ***/
QRegularExpression ChatMessage::_regExpJoin(":(?<name>.*)!.*@.*.tmi.twitch.tv JOIN #.*\\r\\n");
/*** PART ***/
QRegularExpression ChatMessage::_regExpPart(":(?<name>.*)!.*@.*.tmi.twitch.tv PART #.*\\r\\n");
/*** MODE ***/
QRegularExpression ChatMessage::_regExpMode(":jtv MODE #.* \\+o (?<name>.*)\\r\\n");
/*** UNMODE ***/
QRegularExpression ChatMessage::_regExpUnmode(":jtv MODE #.* -o (?<name>.*)\\r\\n");
/*** BITS ***/
QRegularExpression ChatMessage::_regExpBits("@badges=.*;bits=(?<bits>.*);color=(?<color>.*);"
                                            "display-name=(?<author>.*);.*emotes=.*;id=.*;"
                                            "mod=(?<mod>\\d);room-id=.*;.*subscriber=(?<sub>\\d);"
                                            ".*;turbo=\\d;user-id=.*;user-type=.* "
                                            ":(?<name>.*)!.*@.*.tmi.twitch.tv PRIVMSG #.* :(?<msg>.*)\r\n");
/*** PRIVMSG ***/
QRegularExpression ChatMessage::_regExpPrivmsg("@badges=.*;color=(?<color>.*);display-name=(?<author>.*);.*"
                                               "emotes=.*;id=.*;mod=(?<mod>\\d);"
                                               "room-id=.*;.*subscriber=(?<sub>\\d);.*"
                                               "turbo=\\d;user-id=.*;user-type=.* "
                                               ":(?<name>.*)!.*@.*.tmi.twitch.tv PRIVMSG #.* :(?<msg>.*)\\r\\n");
/*** WHISPER ***/
QRegularExpression ChatMessage::_regExpWhisper("@badges=.*;color=(?<color>.*);display-name=(?<author>.*);"
                                               "emotes=.*;message-id=.*;thread-id=.*;"
                                               "turbo=\\d;user-id=.*;user-type=.* "
                                               ":(?<name>.*)!.*@.*.tmi.twitch.tv WHISPER .* :(?<msg>.*)\\r\\n");

///////////////////////////////////////////////////////////////////////////

ChatMessage::ChatMessage()
{
    _author = "NoAuthor";
    _realName = "NoName";
    _color = "Black";
    _timeStamp = "0:0:0";
    _message = "No message";
    _isModerator = false;
    _isSubscriber = false;
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
    return (channelName == _realName);
}

///////////////////////////////////////////////////////////////////////////

MessageType ChatMessage::GetType() const
{
    return _type;
}

///////////////////////////////////////////////////////////////////////////

const QString &ChatMessage::GetBits() const
{
    return _bits;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::IsSubscriber() const
{
    return _isSubscriber;
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

void ChatMessage::SetBits(const QString &bits)
{
    _bits = bits;
}

///////////////////////////////////////////////////////////////////////////

void ChatMessage::SetSubscriber(bool subFlag)
{
    _isSubscriber = subFlag;
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
        else if (_IsRoomState(message))
        {
            msgType = ROOMSTATE;
        }
        else if (_IsJoinMsg(message))
        {
            msgType = JOIN;
        }
        else if (_IsPartMsg(message))
        {
            msgType = PART;
        }
        else if (_IsModeMessage(message))
        {
            msgType = MODE;
        }
        else if (_IsUnmodeMessage(message))
        {
            msgType = UNMODE;
        }
        else if (_IsBits(message))
        {
            msgType = BITS;
        }
        else if (_IsPrivMsg(message))
        {
            msgType = PRIVMSG;
        }
        else if (_IsWhisper(message))
        {
            msgType = WHISPER;
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

bool ChatMessage::_IsUserState(const QString &message)
{
    bool result(false);

    // Try to match string with regular expression
    QRegularExpressionMatch match = _regExpUserstate.match(message);
    if (match.hasMatch())
    {
        result = true;
        // Set params
        _color = match.captured("color");
        _author = match.captured("name");
        _isModerator = (match.captured("mod") == "1") ? true : false;
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsRoomState(const QString &message) const
{
    bool result(false);

    QRegularExpressionMatch match = _regExpRoomState.match(message);
    if (match.hasMatch())
    {
        result = true;
        //TODO: Save gotten info
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsJoinMsg(const QString &message)
{
    bool result(false);

    QRegularExpressionMatch match = _regExpJoin.match(message);
    if (match.hasMatch())
    {
        result = true;

        _realName = match.captured("name");
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPartMsg(const QString &message)
{
    bool result(false);

    QRegularExpressionMatch match = _regExpPart.match(message);
    if (match.hasMatch())
    {
        result = true;

        _realName = match.captured("name");
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsModeMessage(const QString &message)
{
    bool result(false);

    QRegularExpressionMatch match = _regExpMode.match(message);
    if (match.hasMatch())
    {
        result = true;

        _realName = match.captured("name");
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsUnmodeMessage(const QString &message)
{
    bool result(false);

    QRegularExpressionMatch match = _regExpUnmode.match(message);
    if (match.hasMatch())
    {
        result = true;

        _realName = match.captured("name");
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsBits(const QString &message)
{
    bool result(false);

    QRegularExpressionMatch match = _regExpBits.match(message);
    if (match.hasMatch())
    {
        result = true;

        // Login
        _realName = match.captured("name");
        // Display name
        _author = match.captured("author");
        // Name color
        _color = "<font color=\"" + match.captured("color") + "\">";
        // Moderator flag
        _isModerator = (match.captured("mod") == "1") ? true : false;
        // Sub flag
        _isSubscriber = (match.captured("sub") == "1") ? true : false;
        // Bits
        _bits = match.captured("bits");
        // Message itself
        _message = match.captured("msg");
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsPrivMsg(const QString &message)
{
    bool result(false);

    QRegularExpressionMatch match = _regExpPrivmsg.match(message);
    if (match.hasMatch())
    {
        result = true;

        // Login
        _realName = match.captured("name");
        // Display name
        _author = match.captured("author");
        // Name color
        _color = "<font color=\"" + match.captured("color") + "\">";
        // Moderator flag
        _isModerator = (match.captured("mod") == "1") ? true : false;
        if (!_isModerator)
        {
            QString name;
            ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, name);
            // If author is a broadcaster set mod flag true
            if (_realName == name)
            {
                _isModerator = true;
            }
        }
        // Sub flag
        _isSubscriber = (match.captured("sub") == "1") ? true : false;
        // Message itself
        _message = match.captured("msg");
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool ChatMessage::_IsWhisper(const QString &message)
{
    bool result(false);

    QRegularExpressionMatch match = _regExpWhisper.match(message);
    if (match.hasMatch())
    {
        result = true;

        // Login
        _realName = match.captured("name");
        // Display name
        _author = match.captured("author");
        // Name color
        _color = "<font color=\"" + match.captured("color") + "\">";
        // Moderator flag
        // TODO: Add mod somehow!
        // Sub flag
        // TODO: Add sub somehow!
        // Message itself
        _message = match.captured("msg");
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
/***************************** Set functions *****************************/
///////////////////////////////////////////////////////////////////////////

void ChatMessage::_SetTimeStamp()
{
    _timeStamp = QTime::currentTime().toString("h:mm:ss");
}

///////////////////////////////////////////////////////////////////////////
