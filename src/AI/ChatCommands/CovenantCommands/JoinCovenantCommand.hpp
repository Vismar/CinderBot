/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <AI/ChatCommands/ChatCommand.hpp>

namespace Command
{

/*!
 * class JoinCovenantCommand
 * Handle joining users to covenants
 */
class JoinCovenantCommand : public ChatCommand
{
public:
    /*!
     * Deafult Constructor
     */
    JoinCovenantCommand();
    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
    void Initialize();
};

}
