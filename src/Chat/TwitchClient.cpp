/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "TwitchClient.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"

using namespace Utils::Configuration;
using namespace Utils::Database;

#define MSG_TIMER_TIME     30000
#define MSG_LIMIT_NON_MODE 20
#define MSG_LIMIT_MODE     100

///////////////////////////////////////////////////////////////////////////

TwitchClient::TwitchClient(QObject *parent) : QObject(parent)
{
    /* Timers */
    // Msg timer. Starts when messages was received
    _msgTimer = new QTimer(this);
    connect(_msgTimer, &QTimer::timeout,
            this,      &TwitchClient::PingTwitch);
    // Ping timer. Start when bot has sent ping command
    _pingTimer = new QTimer(this);
    connect(_pingTimer, &QTimer::timeout,
            this,       &TwitchClient::Disconnect);

    // Msg timer. Starts immediately
    _msgLimitTimer = new QTimer(this);
    connect(_msgLimitTimer, &QTimer::timeout,
            this,           &TwitchClient::ResetMsgLimit);
    _msgLimitTimer->start(MSG_TIMER_TIME);

    // Message counter
    _msgCounter = 0;
    // Message limit always should be set to minimal in start of client
    _msgLimit = MSG_LIMIT_NON_MODE;

    // Create socket and botAi
    _socket = new QTcpSocket();
    _bot = new BotAI(this);
    // Connections between socket and twitch client
    connect(_socket, &QTcpSocket::stateChanged,
            this,    &TwitchClient::HandleStateChange);
    connect(_socket, &QTcpSocket::disconnected,
            this,    &TwitchClient::Connect);
    connect(_socket, &QTcpSocket::readyRead,
            this,    &TwitchClient::ReadLine);
    // Connection between botAi and twitch client
    connect(_bot,    &BotAI::NewBotMessage,
            this,    &TwitchClient::NewBotMessage);
    connect(this,    &TwitchClient::NewMessage,
            _bot,    &BotAI::ReadNewMessage);
}

///////////////////////////////////////////////////////////////////////////

TwitchClient::~TwitchClient() {}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::Connect()
{    
    _socket->close();
    ConnectionStateChanged(Connecting);
    _socket->connectToHost("irc.twitch.tv", 6667);
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::Disconnect()
{
    ConnectionStateChanged(NoConnection);
    _socket->disconnectFromHost();
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::PingTwitch()
{
    // Try to ping twitch to prevent lost connection when chat is inactive
    _SendIrcMessage("PING\r\n");
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::ReadLine()
{
    ChatMessage chatMessage;

    // If message was received, timer should be stopped
    _msgTimer->stop();
    QString line;
    while (_socket->canReadLine())
    {
        line = _socket->readLine();
        qDebug() << "<-- " << line;
        ChatMessage message;
        // Parse message and get type of it
        MessageType msgType = message.ParseRawMessage(line);
        switch(msgType)
        {
        case INFO:
            emit NewMessage(message, true);
            break;
        case PING:
            _SendIrcMessage("PONG tmi.twitch.tv\r\n");
            break;
        case PONG:
            _pingTimer->stop();
            break;
        case LOGIN_OK:
            _JoinChannel();
            break;
        case USERSTATE:
            // Check if display name was changed
            ConfigurationManager::Instance().GetStringParam(CfgParam::LoginDisplayName, line);
            if (line != message.GetAuthor())
            {
                ConfigurationManager::Instance().SetStringParam(CfgParam::LoginDisplayName, message.GetAuthor());
            }
            // Check if name color was changed
            ConfigurationManager::Instance().GetStringParam(CfgParam::LoginNameColor, line);
            if (line != message.GetColor())
            {
                ConfigurationManager::Instance().SetStringParam(CfgParam::LoginNameColor, message.GetColor());
            }
            // Update mod status
            if (message.IsModerator())
            {
                _msgLimit = MSG_LIMIT_MODE;
            }
            else
            {
                _msgLimit = MSG_LIMIT_NON_MODE;
            }
            break;
        case ROOMSTATE:
            ConnectionStateChanged(Connected);
            break;
        case JOIN:
            UserDataDBHelper::Instance().AddRealTimeUser(message.GetRealName());
            break;
        case PART:
            UserDataDBHelper::Instance().RemoveRealTimeUser(message.GetRealName());
            break;
        case MODE:
            UserDataDBHelper::Instance().AddModerator(message.GetRealName());
            ConfigurationManager::Instance().GetStringParam(CfgParam::LoginName, line);
            if (message.GetRealName() == line)
            {
                _msgLimit = MSG_LIMIT_MODE;
            }
            break;
        case UNMODE:
            UserDataDBHelper::Instance().RemoveModerator(message.GetRealName());
            ConfigurationManager::Instance().GetStringParam(CfgParam::LoginName, line);
            if (message.GetRealName() == line)
            {
                _msgLimit = MSG_LIMIT_NON_MODE;
            }
            break;
        case PRIVMSG:
            emit NewMessage(message, false);
            break;
        case BITS:
            emit NewMessage(message, false);
            break;
        case WHISPER:
            emit NewMessage(message, false);
            break;
        default:
            break;
        }
    }
    // Start timer before sending PING command, that will prevent lost connection if chat is inactive
    _msgTimer->start(45000); // 45 sec
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::HandleStateChange(QAbstractSocket::SocketState state)
{
    ChatMessage message;
    switch (state)
    {
    case QAbstractSocket::ConnectingState:
        message.ParseRawMessage("INFO Connecting to server...");
        emit NewMessage(message, true);
        break;
    case QAbstractSocket::ConnectedState:
        message.ParseRawMessage("INFO Connection established!<br>Authorizing...");
        emit NewMessage(message, true);
        _Login();
        break;
    case QAbstractSocket::UnconnectedState:
        break;
    default:
        qDebug() << state;
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::NewBotMessage(ChatAnswer message)
{
    _SendChatMessage(message);

    // We should not display whispers in chat
    if ((message.GetType() != Twitch_Whisper) && (_msgCounter < _msgLimit))
    {
        ChatMessage botMessage;
        QString loginName;
        QString loginAuthor;
        QString loginNameColor;
        ConfigurationManager::Instance().GetStringParam(CfgParam::LoginName, loginName);
        ConfigurationManager::Instance().GetStringParam(CfgParam::LoginDisplayName, loginAuthor);
        ConfigurationManager::Instance().GetStringParam(CfgParam::LoginNameColor, loginNameColor);

        // Need to add additional delay, because if all timers will end in the same time,
        // it is possible that last one would be checked first, so the last message will be displayed
        // in the first place, and the first message will become last
        int additionalDelay(0);
        QStringList& answers = message.GetAnswers();
        for (auto iter = answers.begin(); iter != answers.end(); ++iter)
        {
            // Prepare chat message
            botMessage.SetAuthor(loginAuthor);
            botMessage.SetRealName(loginName);
            botMessage.SetColor(loginNameColor);
            botMessage.SetMessage(*iter);
            botMessage.SetModFlag(_msgLimit == MSG_LIMIT_MODE);
            // Create single shot timer to emit signal to display bot message with small delay
            QTimer::singleShot(50+additionalDelay, this, [this, botMessage](){ emit NewMessage(botMessage, true); });
            additionalDelay += 10;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::ResetMsgLimit()
{
    _msgCounter = 0;
    _msgLimitTimer->start(MSG_TIMER_TIME);
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::_Login()
{
    QString param;
    QString line;
    // Try to get oauthkey
    if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginOauthKey, param))
    {
        param = "oauth:" + param;
        // Send oauth key
        line = "PASS " + param + "\r\n";
        _SendIrcMessage(line);
        // Try to get login name
        if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginName, param))
        {
            // Send login name
            line = "NICK " + param + "\r\n";
           _SendIrcMessage(line);
            line = "USER " + param + " 8 * :" + param + "\r\n";
            _SendIrcMessage(line);
            // Request useful things
            _SendIrcMessage("CAP REQ :twitch.tv/membership\r\n");
            _SendIrcMessage("CAP REQ :twitch.tv/tags\r\n");
            _SendIrcMessage("CAP REQ :twitch.tv/commands\r\n");
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::_JoinChannel()
{
    QString param;
    // Try to get channel name
    if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginChannel, param))
    {
        param.push_front("JOIN #");
        param.push_back("\r\n");
        _SendIrcMessage(param);
    }
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::_SendIrcMessage(const QString &message)
{
    _socket->write(message.toStdString().c_str());
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::_SendChatMessage(ChatAnswer &message)
{
    QStringList& answers = message.GetAnswers();
    if ((_msgCounter + answers.size()) < _msgLimit)
    {
        QString param;
        // Try to get channel name
        ChatAnswerType answerType = message.GetType();
        if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginChannel, param))
        {
            for (auto iter = answers.begin(); iter != answers.end(); ++iter)
            {
                QString msg("PRIVMSG #" + param);
                msg.push_back(" :");
                if (answerType == Twitch_Whisper)
                {
                    msg.push_back(QString("/w %1 ").arg(message.GetRealName()));
                }
                msg.push_back(*iter);
                msg.push_back("\r\n");
                _SendIrcMessage(msg);
                qDebug() << "--> " << msg;
            }
        }
        switch (answerType)
        {
        case Twitch_Chat:
            ++_msgCounter;
            break;
        case Twitch_Whisper:
            ++_whisperCounter;
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
