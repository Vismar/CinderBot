/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCovChatCommand.hpp"

using namespace Command::CustomChatCmd;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CustomCovChatCommand::CustomCovChatCommand()
{
    _commandTableName = "CustomCovCommands";
    _commandAnswersTableName = "CustomCovCommandAnswers";
    _cmdType = CmdType::CovenantCmd;
}

///////////////////////////////////////////////////////////////////////////
