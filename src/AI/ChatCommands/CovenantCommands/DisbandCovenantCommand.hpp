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
 * Class JoinCovenantCommand
 * Handle disbanding the covenant by user
 */
class DisbandCovenantCommand : public ChatCommand
{
public:
    /*!
     * Deafult Constructor
     */
    DisbandCovenantCommand();
    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
    void Initialize();
};

}
