/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UserDataTimerCommandList.hpp"
#include "CurrencyToOnlineTimerCommand.hpp"

using namespace TimerCommand::UserDataTimerCmd;

///////////////////////////////////////////////////////////////////////////

UserDataTimerCommandList::UserDataTimerCommandList()
{
    _timerCommands.push_back(new CurrencyToOnlineTimerCommand());
}

///////////////////////////////////////////////////////////////////////////

UserDataTimerCommandList::~UserDataTimerCommandList() { }

///////////////////////////////////////////////////////////////////////////
