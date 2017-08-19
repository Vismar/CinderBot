/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCovCommandList.hpp"
#include "AI/ChatCommands/CustomCommands/CustomCovChatCommand.hpp"

using namespace Command::CustomChatCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CustomCovCommandList::CustomCovCommandList()
{
    _commandType = CmdType::CovenantCmd;
    _cmdModule = CfgParam::CovenantCmdModule;
}

///////////////////////////////////////////////////////////////////////////

CustomChatCommand* CustomCovCommandList::_CreateCommand() const
{
    return new CustomCovChatCommand();
}

///////////////////////////////////////////////////////////////////////////
