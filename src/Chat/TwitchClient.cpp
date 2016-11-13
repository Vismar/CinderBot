#include "TwitchClient.hpp"
#include "../Utils/Config/ConfigurationManager.hpp"
#include "../Utils/Config/ConfigurationParameters.hpp"

TwitchClient::TwitchClient(QObject *parent) : QObject(parent)
{
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

void TwitchClient::ReadLine()
{
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
        case PRIVMSG:
            emit NewMessage(message, false);
            break;
        case LOGIN_OK:
            JoinChannel();
            break;
        default:
            break;
        }
    }
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
