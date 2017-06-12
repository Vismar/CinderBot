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
 * class InfoCovenantCommand
 * Display info about specified covenant
 */
class InfoCovenantCommand : public InbuiltChatCommand
{
public:
    /*!
     * Deafult Constructor
     */
    InfoCovenantCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage &message, QStringList &answer);
    void _GetRandomAnswer(const ChatMessage &message, QStringList &answer);
};

}
