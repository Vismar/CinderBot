#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QString>
#include "ChatMessage.hpp"
#include "../AI/BotAI.hpp"

class TwitchClient : public QObject
{
    Q_OBJECT
private:
    BotAI*      _bot;
    QTcpSocket* _socket;
    QString     _address;
    int         _port;
    QString     _room;

public:
    explicit TwitchClient(QObject *parent = 0);
    ~TwitchClient();

    void Connect();
    void Login();
    void JoinChannel();

private:
    void _SendIrcMessage(const QString& message);
    void _SendChatMessage(const QString& message);

signals:
    void NewMessage(ChatMessage message, bool botMessage);

public slots:
    /*void DisconnectChannel();
    void Disconnect();
    */
    void ReadLine();
    void HandleStateChange(QAbstractSocket::SocketState state);
    void NewBotMessage(QString message);
};
