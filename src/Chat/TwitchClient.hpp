/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTimer>
#include "ChatMessage.hpp"
#include "AI/BotAI.hpp"
#include "Chat/ChatAnswer.hpp"

using namespace Chat;

enum ConnectionState
{
    NoConnection,
    Connecting,
    Connected
};

/*!
 * Class TwtichClient
 * Handles all network stuff and include BotAI.
 */
class TwitchClient : public QObject
{
    Q_OBJECT
public:
    explicit TwitchClient(QObject *parent = 0);
    ~TwitchClient();

signals:
    /*!
     * Special event that emits to notify listeners about new chat message
     * \param(IN) message - new message
     * \param(IN) botMessage - flag that marks message as bot message(or not)
     */
    void NewMessage(ChatMessage message, bool botMessage);

    void ConnectionStateChanged(ConnectionState state);

public slots:
    /*!
     * Close socket and trying to connect to irc.twitch.tv
     */
    void Connect();
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
    void NewBotMessage(ChatAnswer message);
    /*!
     * Reset msgLimit
     */
    void ResetMsgLimit();

private:
    /*!
     * Sends authentication messages to twitch
     */
    void _Login();
    /*!
     * Sends JOIN command to channel
     */
    void _JoinChannel();

    /*!
     * Sends raw message to twitch
     * \param(IN) message - raw data
     */
    void _SendIrcMessage(const QString &message);
    /*!
     * Generates raw data that will be sent to channel via _SendIrcMessage
     * \param(IN) message - message that should be sent to channel
     */
    void _SendChatMessage(ChatAnswer &message);

    /*! BotAI. No direct calls, only signal-slot connections. */
    BotAI*      _bot;
    /*! Tcp socket. Handles connection. */
    QTcpSocket* _socket;
    /*! Message timer that starts after last received message */
    QTimer*     _msgTimer;
    /*! Ping timer that start after ping command that was sent to twitch */
    QTimer*     _pingTimer;
    /*! Timer which will cause reseting _msgCounter */
    QTimer*     _msgLimitTimer;
    /*! Number of messages that was sent to twitch */
    int         _msgCounter;
    /*! Number of messages that was sent to twitch via whispers */
    int         _whisperCounter;
    /*! Limit of messages that can be sent to twitch */
    int         _msgLimit;
};
