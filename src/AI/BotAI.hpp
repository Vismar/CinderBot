/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include "Chat/ChatMessage.hpp"
#include "Chat/ChatAnswer.hpp"
#include "AI/ChatCommands/CommandList.hpp"
#include "AI/TimerCommands/BaseTimerCommandList.hpp"

/*!
 * \brief Stores commands and handles its execution.
 *
 * Manages all "intelligence" of the bot. Chat commands, timer command, etc.
 */
class BotAI : public QObject
{
    Q_OBJECT
public:
    explicit BotAI(QObject *parent = nullptr);
    ~BotAI();

signals:
    /*!
     * \brief Signal about new bot message.
     * \param message - bot message.
     *
     * Signal about new bot message which should be rendered in chat.
     */
    void NewBotMessage(ChatAnswer message);

public slots:
    /*!
     * \brief Read and process new message.
     * \param message - chat message.
     * \param botMessage - is it a bot or not?
     *
     * Sends to chat commands and try to find command in message to execute it.
     */
    void ReadNewMessage(ChatMessage message, bool botMessage);

private:
    /*!
     * \brief Load all chat commands.
     *
     * All chat commands should be loaded here.
     */
    void LoadCommands();
    /*!
     * \brief Load all timer commands.
     *
     * All timer commands should be loaded here.
     */
    void LoadTimerCommands();

    /*!
     * \brief Checks if user is in ignore list.
     * \param userName - name of a user.
     * \return true if user is in ignore list.
     *
     * Checks if specified user if in ignore list.
     */
    bool _CheckIgnoreList(const QString &userName);
    /*!
     * \brief Updates user data.
     * \param message - chat message.
     *
     * Updates all user data: author, message counter and currency.
     */
    void _UpdateUserData(const ChatMessage &message);

    /*! Array of commands */
    QVector<Command::CommandList*> _chatCommands;
    /*! Array of timer commands */
    QVector<TimerCommand::BaseTimerCommandList*> _timerCommands;
    /*! Ignore list. Commands from users that stored in this list will not be executed */
    QStringList _ignoreList;
};
