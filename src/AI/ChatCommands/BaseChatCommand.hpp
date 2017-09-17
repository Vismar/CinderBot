/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Chat/ChatMessage.hpp"
#include "Chat/ChatAnswer.hpp"
#include <QDateTime>
#include <QObject>

using namespace Chat;

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{

/*!
 * \brief Base chat command which provides base architecture and functionality for all chat commands.
 * 
 * Provides basic functionality to check existence of executing command in message
 * and executing it by one of two methods. Also, handles replacing placeholders which can be used by any command.
 */
class BaseChatCommand : public QObject
{
    Q_OBJECT
public:
    BaseChatCommand();
    virtual ~BaseChatCommand();

    /*!
     * \brief Initializes command by its own way. 
     * 
     * Should be implemented by inherited class to initialize in a proper way.
     */
    virtual void Initialize() = 0;

    /*!
     * \brief Tries to execute command.
     * \param message - chat message.
     * \param answer - reference to variable which will store answer.
     * \return true if command was executed.
     * 
     * Checks every base thing before execution and then tries ti get answer. 
     * If command was executed, reaplaces all placeholders.
     */
    bool Execute(const ChatMessage &message, ChatAnswer &answer);

    /*!
     * \brief Returns command name.
     * \return Command name which represented as QString.
     */
    const QString &GetCommandName() const;

protected:
    /*!
     * \brief Get certain answer which depends on some conditions.
     * \param message - chat message.
     * \param answer - reference to variable which will store answer.
     * 
     * Should be implemented by inherited class to provide needed answer. 
     * This function called only if _isRandom setted to "false".
     */
    virtual void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) = 0;
    /*!
     * \brief Get random answer
     * \param message - chat message
     * \param answer - reference to variable which will store answer
     * 
     * Should be implemented by inherited class to provide needed answer. 
     * This function called only if _isRandom setted to "true".
     */
    virtual void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) = 0;

    /* Utility functions */
    /*!
     * \brief Clears all saved data.
     * 
     * Resets all command params to default.
     */
    void _Clear();
    /*!
     * \brief Replace all placeholders which was used in answer.
     * \param message - chat message.
     * \param answer - reference to variable which will store answer.
     * 
     * Calls all "replace" function to replace placeholders by proper data.
     */
    void _ReplacePlaceHolders(const ChatMessage &message, QStringList &answer);
    /*!
     * \brief Try to find "@" symbol and attach author name to it.
     * \param answer - selected answer that will be parsed to add author name.
     * \param author - author name.
     */
    void _AddAuthorName(QStringList &answer, const QString &author);
    /*!
     * Take default specififed price from user
     * \param userName - name of user
     */
    void _TakeDefaultPriceFromUser(const QString &userName) const;
    /*!
     * \brief Take specified price from user.
     * \param userName - name of user
     * \param price - price which should be taken from user.
     */
    void _TakePriceFromUser(const QString &userName, int price);

    /* Flag checkers */
    /*!
     * \brief Checks if command can be executed for specified message type.
     * \param msgType - type of incoming message.
     */
    bool _IsAnswerAllowed(MessageType msgType);
    /*!
     * \brief Checks if user can execute command if covenant restriction was specified
     * \param userName - real name of user
     * \return true if user can use command
     */
    bool _CheckCovenant(const QString &userName);
    /*!
     * \brief Checks if user have enough currency to use command.
     * \param userName - real name of user.
     * \return true if user can use command.
     */
    bool _CheckCurrency(const QString &userName);
    /*!
     * \brief Checks if command cooldown was expired.
     * \return true if user can use command.
     */
    bool _CheckCooldown();
    /*!
     * \brief Checks if user can execute command depends on moderator only flag.
     * \param userIsModerator - falg which indicates if user moderator or not.
     * \return true if user can use command.
     * 
     * If flag _modOnly flag setted to "true" and user is moderator, then command will be executed.
     * Otherwise - will not. Same goes to vice versa.
     */
    bool _CheckModerationFlag(bool userIsModerator);

    /* Variables */
    /*! Flag which represnts possibility of execution command via whisper message */
    bool _workInWhisper;
    /*! Flag which represnts possibility of execution command via chat message */
    bool _workInChat;
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
