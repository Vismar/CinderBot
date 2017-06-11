/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "TwitchClient.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include "Utils/UserData/RealTimeUserData.hpp"

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
    // Try to connect to twitch
    Connect();
    // Connections between socket and twitch client
    connect(_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,    SLOT(HandleStateChange(QAbstractSocket::SocketState)));
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
    _socket->connectToHost("irc.twitch.tv", 6667);
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::Login()
{
    QString param;
    QString line;
    // Try to get oathkey
    if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_OATH_KEY, param))
    {
        // Send oath key
        line = "PASS " + param + "\r\n";
        _SendIrcMessage(line);
        // Try to get login name
        if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, param))
        {
            // Send login name
            line = "NICK " + param + "\r\n";
           _SendIrcMessage(line);
            line = "USER " + param + " 8 * :" + param + "\r\n";
            _SendIrcMessage(line);
            // Requst useful things
            _SendIrcMessage("CAP REQ :twitch.tv/membership\r\n");
            _SendIrcMessage("CAP REQ :twitch.tv/tags\r\n");
            _SendIrcMessage("CAP REQ :twitch.tv/commands\r\n");
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::JoinChannel()
{
    QString param;
    // Try to get channel name
    if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, param))
    {
        param.push_front("JOIN #");
        param.push_back("\r\n");
        _SendIrcMessage(param);
    }
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::Disconnect()
{
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
    // If message was received, timer should be stoped
    _msgTimer->stop();
    QString line;
    while (_socket->canReadLine())
    {
        line = _socket->readLine();
        qDebug() << line;
        ChatMessage message;
        // Parse message and get type of it
        MessageType msgType = message.ParseRawMessage(line);
        switch(msgType)
        {
        case INFO:
            if (message.GetMessage() == SYSTEM_MESSAGE_CR)
            {
                QStringList hiMsg;
                hiMsg.append("Poooound Всем приветище! Poooound");
                _SendChatMessage(hiMsg);
            }
            emit NewMessage(message, true);
            break;
        case PING:
            _SendIrcMessage("PONG tmi.twitch.tv\r\n");
            break;
        case PONG:
            _pingTimer->stop();
            break;
        case LOGIN_OK:
            JoinChannel();
            break;
        case USERSTATE:
            break;
        case JOIN:
            RealTimeUserData::Instance()->AddUserToList(message);
            break;
        case PART:
            RealTimeUserData::Instance()->RemoveUserFromList(message);
            break;
        case MODE:
            RealTimeUserData::Instance()->AddModeToList(message);
            ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, line);
            if (message.GetRealName() == line.toLower())
            {
                _msgLimit = MSG_LIMIT_MODE;
            }
            break;
        case UNMODE:
            RealTimeUserData::Instance()->RemoveModeFromList(message);
            ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, line);
            if (message.GetRealName() == line.toLower())
            {
                _msgLimit = MSG_LIMIT_NON_MODE;
            }
            break;
        case PRIVMSG:
            RealTimeUserData::Instance()->IncrementMsgCounter();
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
        Login();
        break;
    case QAbstractSocket::UnconnectedState:
        break;
    default:
        qDebug() << state;
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::NewBotMessage(QStringList message)
{
    _SendChatMessage(message);
    if ((_msgCounter + message.size()) < _msgLimit)
    {
        ChatMessage botMessage;
        QString param;
        // Try to get login name
        if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, param))
        {
            // Need to add additional delay, because if all timers will end in the same time,
            // it is possible that last one would be checked first, so the last message will be displayed
            // in the first place, and the first message will become last
            int additionalDelay(0);
            for (auto iter = message.begin(); iter != message.end(); ++iter)
            {
                // Prepare chat message
                botMessage.SetAuthor(param);
                botMessage.SetRealName(param);
                botMessage.SetColor("Green");
                botMessage.SetMessage(*iter);
                botMessage.SetModFlag(_msgLimit == MSG_LIMIT_MODE);
                // Create single shot timer to emit signal to display bot message with small delay
                QTimer::singleShot(50+additionalDelay, this, [this, botMessage](){ emit NewMessage(botMessage, true); });
                additionalDelay += 10;
            }
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

void TwitchClient::_SendIrcMessage(const QString& message)
{
    _socket->write(message.toStdString().c_str());
}

///////////////////////////////////////////////////////////////////////////

void TwitchClient::_SendChatMessage(const QStringList& message)
{
    if ((_msgCounter + message.size()) < _msgLimit)
    {
        QString param;
        // Try to get channel name
        if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, param))
        {
            for (auto iter = message.begin(); iter != message.end(); ++iter)
            {
                QString msg("PRIVMSG #" + param);
                msg.push_back(" :");
                msg.push_back(*iter);
                msg.push_back("\r\n");
                _SendIrcMessage(msg);
                qDebug() << msg;
            }
        }
        ++_msgCounter;
    }
}

///////////////////////////////////////////////////////////////////////////
