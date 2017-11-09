/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CovenantDataTimerCommandList.hpp"
#include "AI/TimerCommands/CovenantData/CovenantExpTimerCommand.hpp"

using namespace TimerCommand::CovenantDataTimerCmd;

///////////////////////////////////////////////////////////////////////////

CovenantDataTimerCommandList::CovenantDataTimerCommandList()
{
    _timerCommands.push_back(new CovenantExpTimerCommand());
}

///////////////////////////////////////////////////////////////////////////
