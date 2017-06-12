/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/InbuiltChatCommand.hpp"

namespace Command
{

/*!
 * class JoinCovenantCommand
 * Display covenant list to user
 */
class ListCovenantCommand : public InbuiltChatCommand
{
public:
    /*!
     * Deafult Constructor
     */
    ListCovenantCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage &message, QStringList &answer);
    void _GetRandomAnswer(const ChatMessage &message, QStringList &answer);
};

}
