#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTimer>
#include "ChatMessage.hpp"
#include "../AI/BotAI.hpp"

/*!
 * Class TwtichClient
 * Handles all network stuff and include BotAI.
 */
class TwitchClient : public QObject
{
    Q_OBJECT
private:
    /*! BotAI. No direct calls, only signal-slot connections. */
    BotAI*      _bot;
    /*! Tcp socket. Handles connection. */
    QTcpSocket* _socket;
    /*! Message timer that starts after last received message */
    QTimer*     _msgTimer;
    /*! Ping timer that start after ping command that was sent to twitch */
    QTimer*     _pingTimer;

public:
    explicit TwitchClient(QObject *parent = 0);
    ~TwitchClient();

    /*!
     * Close socket and trying to connect to irc.twitch.tv
     */
    void Connect();
    /*!
     * Sends authentication messages to twitch
     */
    void Login();
    /*!
     * Sends JOIN command to channel
     */
    void JoinChannel();

private:
    /*!
     * Sends raw message to twitch
     * \param(IN) message - raw data
     */
    void _SendIrcMessage(const QString& message);
    /*!
     * Generates raw data that will be sent to channel via _SendIrcMessage
     * \param(IN) message - message that should be sent to channel
     */
    void _SendChatMessage(const QString& message);

signals:
    /*!
     * Special event that emits to notify listeners about new chat message
     * \param(IN) message - new message
     * \param(IN) botMessage - flag that marks message as bot message(or not)
     */
    void NewMessage(ChatMessage message, bool botMessage);

public slots:
    /*void LeaveChannel();*/
    /*!
     * Disconnect from host
     */
    void Disconnect();
    /*!
     * Sends ping command to twitch
     */
    void PingTwitch();
    /*!
     * Read all lines that comes to socket. Parse it via ChatMessage parser and decides what to do
     */
    void ReadLine();
    /*!
     * Handles socket connection state events
     */
    void HandleStateChange(QAbstractSocket::SocketState state);
    /*!
     * Sends bot message to twitch and emit signal about new message
     */
    void NewBotMessage(QString message);
};
