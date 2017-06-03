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
 * class RenameCovenantCommand
 * Handle renaming covenants by users
 */
class RenameCovenantCommand : public InbuiltChatCommand
{
public:
    /*!
     * Deafult Constructor
     */
    RenameCovenantCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage& message, QString& answer);
    void _GetRandomAnswer(const ChatMessage& message, QString& answer);
};

}
