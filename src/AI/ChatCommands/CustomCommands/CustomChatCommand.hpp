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
namespace CustomChatCmd
{

class CustomChatCommand : public BaseChatCommand
{
public:
    CustomChatCommand();
    virtual ~CustomChatCommand();

    ////////////////////////////////
    /// BaseChatCommand overrides
    virtual void Initialize();

    /*!
     * Inititalize command from database by specified name.
     * For initialization should be used only that function.
     * \param commandName - name of command
     */
    void InitializeByName(const QString &commandName);

public slots:
    void OnParameterChanged(const QString &cmdName, Utils::Database::CustomCmdParameter cmdParam, const QString &value);

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    virtual void _GetAnswer(const ChatMessage &message, ChatAnswer &answer);
    virtual void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer);

    /*! Name of table from which data should be grabbed */
    QString _commandTableName;
    /*! Name of table from which answers should be grabbed */
    QString _commandAnswersTableName;
    /*! Type of a command */
    Utils::Database::CmdType _cmdType;
};

}
}
