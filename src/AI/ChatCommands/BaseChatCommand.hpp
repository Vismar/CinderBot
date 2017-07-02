/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Chat/ChatMessage.hpp"
#include "Chat/ChatAnswer.hpp"
#include <QStringList>
#include <QTime>
#include <QDateTime>

namespace Command
{

/*!
 * class BaseChatCommand
 * Provide basic functionality to check existence of executing command in message
 * and executing it by one of two methods. Also, handles replacing placeholders which can be used by any command.
 */
class BaseChatCommand
{
public:
    /*! Constructor */
    BaseChatCommand();
    /*! Destructor */
    virtual ~BaseChatCommand();
    /*!
     * Initialize command by your own way. Should be implemented in inherited class
     */
    virtual void Initialize() = 0;

    /*!
     * Tries to execute command
     * \param message - chat message
     * \param answer - reference to variable which will store answer
     * \return true if command was executed
     */
    bool Execute(const ChatMessage &message, ChatAnswer &answer);

protected:
    /*!
     * Get answer which depends on some conditions
     * \param message - chat message
     * \param answer - reference to variable which will store answer
     */
    virtual void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) = 0;
    /*!
     * Get random answer
     * \param message - chat message
     * \param answer - reference to variable which will store answer
     */
    virtual void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) = 0;

    /* Utility functions */
    /*!
     * Clears all saved data
     */
    void _Clear();
    /*!
     * Replace all placeholders which was used in answer
     * \param message - chat message
     * \param answer - reference to variable which will store answer
     */
    void _ReplacePlaceHolders(const ChatMessage &message, QStringList &answer);
    /*!
     * Try to find "@" symbol and attach author name to it
     * \param answer - selected answer that will be parsed to add author name
     * \param author - author name
     */
    void _AddAuthorName(QStringList &answer, const QString &author);
    /*!
     * Take default specififed price from user
     * \param userName - name of user
     */
    void _TakeDefaultPriceFromUser(const QString &userName);
    /*!
     * Take specified price from user
     * \param userName - name of user
     * \param price - price which should be taken from user
     */
    void _TakePriceFromUser(const QString &userName, int price);

    /* Flag checkers */
    /*!
     * Check if user can execute command if covenant restriction was specified
     * \param userName - real name of user
     * \return true if user can use command
     */
    bool _CheckCovenant(const QString &userName);
    /*!
     * Check if user have enough currency to use command
     * \param userName - real name of user
     * \return true if user can use command
     */
    bool _CheckCurrency(const QString &userName);
    /*!
     * Check if command cooldown was expired
     * \return true if user can use command
     */
    bool _CheckCooldown();
    /*!
     * Check if user can execute command depends on moderator only flag
     * \param userIsModerator - falg which indicates if user moderator or not
     * \return true if user can use command
     */
    bool _CheckModerationFlag(bool userIsModerator);

    /* Variables */
    /*! Flag which represents should command return random answer or not */
    bool _isRandom;
    /*! Command name */
    QString _name;
    /*! Time, when command was executed for a last time */
    QDateTime _lastTimeUsed;
    /*! Command cooldown */
    QTime _cooldown;
    /*! If command can be executed only be moderators */
    bool _moderatorOnly;
    /*! Price for executing command */
    int _price;
    /*! Specific covenant, user of which can use this command */
    QString _covenant;
};

}
