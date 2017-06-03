/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <AI/ChatCommands/InbuiltChatCommand.hpp>

namespace Command
{

/*!
 * class JoinCovenantCommand
 * Handle joining users to covenants
 */
class JoinCovenantCommand : public InbuiltChatCommand
{
public:
    /*!
     * Deafult Constructor
     */
    JoinCovenantCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage& message, QString& answer);
    void _GetRandomAnswer(const ChatMessage& message, QString& answer);
};

}
