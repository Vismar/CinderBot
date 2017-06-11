/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include <QVector>
#include <QStringList>
#include <QTimer>
#include "Chat/ChatMessage.hpp"
#include "AI/ChatCommands/CommandList.hpp"

/*!
 * Class BotAI
 * Stores commands and handles its execute
 */
class BotAI : public QObject
{
    Q_OBJECT
public:
    explicit BotAI(QObject* parent = 0);
    ~BotAI();

signals:
    /*!
     * Signal about new bot message
     * \param(IN) message - bot message
     */
    void NewBotMessage(QStringList message);

public slots:
    /*!
     * Read and process new message
     * \param(IN) message - chat message
     * \param(IN) botMessage - is it a bot or not?
     */
    void ReadNewMessage(ChatMessage message, bool botMessage);

    /*!
     * Give defined number of currency to all users,
     * who is in the chat, except for users who is in ignore list
     */
    void GiveCurrencyOnTimer();

private:
    /*!
     * Check if user is in ignore list
     * \param(IN) userName - name of a user
     * \return true if user is in ignore list
     */
    bool _CheckIgnoreList(const QString& userName);
    /*!
     * Update user data (Message counter and currency)
     * \param(IN) message - chat message
     */
    void _UpdateUserData(const ChatMessage& message);
    /*!
     * Increment message counter
     * \param(IN) userName - name of a user
     */
    void _IncrementMessageCounter(const QString& userName);
    /*!
     * Add currency to a user
     * \param(IN) userName - name of a user
     * \param(IN) value - amount of currency that should be added
     */
    void _AddCurrency(const QString& userName, const int value);

    /*! Array of commands */
    QVector<Command::CommandList*> _chatCommands;
    /*! Ignore list. Commands from users that stored in this list will not be executed */
    QStringList _ignoreList;
    /*! Timer which will execute some stuff for every user who is in the chat, except for users from ignore list */
    QTimer _currencyTimer;
};
