/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UtilityCommandList.hpp"
#include "AI/ChatCommands/UtilityCommands/FollowersChatCommand.hpp"

using namespace Command::UtilityCmd;

///////////////////////////////////////////////////////////////////////////

UtilityCommandList::UtilityCommandList()
{
    _commands.push_back(new FollowersChatCommand());
}

///////////////////////////////////////////////////////////////////////////

void UtilityCommandList::OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam)
{
    Q_UNUSED(cfgParam);
}

///////////////////////////////////////////////////////////////////////////
