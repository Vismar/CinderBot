/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "ChatCommand.hpp"

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
     * \param(IN) message - chat message
     * \param(OUT) answer - returning answer
     * \return true if one of commands from command list was executed
     */
    virtual bool TryGetAnswer(const ChatMessage& message, QString& answer);

protected:
    /*!
     * Initialize command list. Should be overridden by derived class.
     */
    virtual void _Initialize();

    /*! List of commands */
    QVector<ChatCommand*> _commands;
};

}
