/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <AI/ChatCommands/BaseChatCommand.hpp>

namespace Command
{

class CustomChatCommand : public BaseChatCommand
{
public:
    /*! Constructor */
    CustomChatCommand();
    /*! Destructor */
    virtual ~CustomChatCommand();

    ////////////////////////////////
    /// BaseChatCommand overrides
    virtual void Initialize();

    /*!
     * Inititalize command from database by specified name.
     * For initialization should be used only that function.
     * \param commandName - name of command
     */
    void InitializeByName(const QString& commandName);

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    virtual void _GetAnswer(const ChatMessage& message, QString& answer);
    virtual void _GetRandomAnswer(const ChatMessage& message, QString& answer);

    /*! Name of table from which data should be grabbed */
    QString _commandTableName;
    /*! Name of table from which answers should be grabbed */
    QString _commandAnswersTableName;
};

}
