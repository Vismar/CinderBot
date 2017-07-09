/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/BaseChatCommand.hpp"
#include "Chat/ChatAnswer.hpp"
#include <QVector>

namespace Command
{

/*!
 * Simple and base command list.
 */
class CommandList
{
public:
    /*! Destructor */
    virtual ~CommandList();
    /*!
     * Try to get answer from stored list of commands
     * \param message - chat message
     * \param answer - returning answer
     * \return true if one of commands from command list was executed
     */
    virtual bool TryExecute(const ChatMessage &message, ChatAnswer &answer);
    /*!
     * Initialize command list. Should be overridden by derived class.
     */
    virtual void Initialize() = 0;

protected:
    /*! List of commands */
    QVector<BaseChatCommand*> _commands;
};

}
