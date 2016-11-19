#include "TwitchClient.hpp"
#include "../Utils/Config/ConfigurationManager.hpp"
#include "../Utils/Config/ConfigurationParameters.hpp"

TwitchClient::TwitchClient(QObject *parent) : QObject(parent)
{
    /*** Timers ***/
    // Msg timer. Starts when messages was received
    _msgTimer = new QTimer(this);
    connect(_msgTimer, &QTimer::timeout,
            this,      &TwitchClient::PingTwitch);
    // Ping timer. Start when bot has sent ping command
    _pingTimer = new QTimer(this);
    connect(_pingTimer, &QTimer::timeout,
            this,       &TwitchClient::Disconnect);

    _socket = new QTcpSocket();
    _bot = new BotAI(this);
    Connect();
    connect(_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,    SLOT(HandleStateChange(QAbstractSocket::SocketState)));
    connect(_socket, &QTcpSocket::disconnected,
            this,    &TwitchClient::Connect);
    connect(_socket, &QTcpSocket::readyRead,
            this,    &TwitchClient::ReadLine);
    connect(_bot,    &BotAI::NewBotMessage,
            this,    &TwitchClient::NewBotMessage);
    connect(this,    &TwitchClient::NewMessage,
            _bot,    &BotAI::ReadNewMessage);
}

TwitchClient::~TwitchClient() {}

void TwitchClient::Connect()
{
    _socket->close();
    _socket->connectToHost("irc.twitch.tv", 6667);
}

void TwitchClient::Login()
{
    QString param;
    QString line;
    if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_OATH_KEY, param))
    {
        line = "PASS " + param + "\r\n";
        _SendIrcMessage(line);
        if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, param))
        {
            line = "NICK " + param + "\r\n";
           _SendIrcMessage(line);
            line = "USER " + param + " 8 * :" + param + "\r\n";
            _SendIrcMessage(line);
            _SendIrcMessage("CAP REQ :twitch.tv/membership\r\n");
            _SendIrcMessage("CAP REQ :twitch.tv/tags\r\n");
            _SendIrcMessage("CAP REQ :twitch.tv/commands\r\n");
        }
    }
}

void TwitchClient::JoinChannel()
{
    QString param;
    if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, param))
    {
        param.push_front("JOIN #");
        param.push_back("\r\n");
        _SendIrcMessage(param);
    }
}

void TwitchClient::_SendIrcMessage(const QString& message)
{
    _socket->write(message.toStdString().c_str());
}

void TwitchClient::_SendChatMessage(const QString& message)
{
    QString param;
    if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, param))
    {
        param.push_front("PRIVMSG #");
        param.push_back(" :");
        param.push_back(message);
        param.push_back("\r\n");
        _SendIrcMessage(param);
    }
}

void TwitchClient::Disconnect()
{
    _socket->disconnectFromHost();
}

void TwitchClient::PingTwitch()
{
    // Try to ping twitch to prevent lost ceonnection when chat is inactive
    _SendIrcMessage("PING\r\n");
}

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
        MessageType msgType = message.ParseRawMessage(line);
        switch(msgType)
        {
        case INFO:
            if (message.GetMessage() == SYSTEM_MESSAGE_CR)
            {
                //TODO: Should be changed to selected phrase from dictionary
                _SendChatMessage("Poooound Всем приветище! Poooound");
            }
            emit NewMessage(message, true);
            break;
        case PING:
            _SendIrcMessage("PONG tmi.twitch.tv\r\n");
            break;
        case PONG:
            _pingTimer->stop();
            break;
        case USERSTATE:
            break;
        case LOGIN_OK:
            JoinChannel();
            break;
        case PRIVMSG:
            emit NewMessage(message, false);
            break;
        default:
            break;
        }
    }
    // Start timer before sending PING command, that will prevent lost connection if chat is inactive
    _msgTimer->start(45000); // 45 sec
}

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

void TwitchClient::NewBotMessage(QString message)
{
    _SendChatMessage(message);
    ChatMessage botMessage;
    QString param;
    if (ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, param))
    {
        botMessage.SetAuthor(param);
        botMessage.SetColor("Green");
        botMessage.SetMessage(message);
        emit NewMessage(botMessage, true);
    }
}
