/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/InbuiltChatCommand.hpp"

namespace Command
{
namespace CovenantCmd
{

/*!
 * class DescriptionCovenantCommand
 * Handle creating and editing covenant description by users
 */
class DescriptionCovenantCommand : public InbuiltChatCommand
{
public:
    /*!
     * Deafult Constructor
     */
    DescriptionCovenantCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer);
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer);
};

}
}
