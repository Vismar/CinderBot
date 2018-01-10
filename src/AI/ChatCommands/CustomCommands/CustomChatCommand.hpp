/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/BaseChatCommand.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{
/*!
 * \brief Contains all custom chat commands.
 */
namespace CustomChatCmd
{

/*!
 * \brief Chat command to store custom command created by user.
 * 
 * Chat command initializes from database to store custom command created by user.
 * Updates params via events, always returns random answer.
 */
class CustomChatCommand : public BaseChatCommand
{
public:
    CustomChatCommand();
    virtual ~CustomChatCommand();

    /*////////////////////////////*/
    /* BaseChatCommand overrides  */
    /*////////////////////////////*/
    /*!
     * \brief Initializes parameters for certain command.
     */
    void Initialize() override;

    /*!
     * \brief Initializes command from database by specified name.
     * \param commandName - name of command
     * 
     * Resets all params to default state and calls Initialize(). For initialization should be used only this function.
     */
    void InitializeByName(const QString &commandName);

public slots:
    /*!
     * \brief Updates specified parameter if specified command name equals to stored value.
     */
    void OnParameterChanged(const QString &cmdName, Utils::Database::CustomCmdParameter cmdParam, const QString &value);

protected:
    /*!
     * \brief Connects events for custom commands.
     */
    virtual void _ConnectEvents();

    /*////////////////////////////*/
    /* BaseChatCommand overrides  */
    /*////////////////////////////*/
    /*!
     * \brief Empty function, do nothing.
     */
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) override;
    /*!
     * \brief Gets random answer for current command.
     *  
     * Gets random answer by CustomCommandDBHelper and splits it in multiple messages 
     * if answer is to big (more than 500 symbols).
     */
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) override;

    /*! Name of table from which data should be grabbed */
    QString _commandTableName;
    /*! Name of table from which answers should be grabbed */
    QString _commandAnswersTableName;
    /*! Type of a command */
    Utils::Database::CmdType _cmdType;
};

}
}
