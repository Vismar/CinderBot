/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "BaseTimerCommandList.hpp"

using namespace TimerCommand;

///////////////////////////////////////////////////////////////////////////

BaseTimerCommandList::BaseTimerCommandList() : QObject(0) { }

///////////////////////////////////////////////////////////////////////////

BaseTimerCommandList::~BaseTimerCommandList()
{
    for (int i = 0; i < _timerCommands.size(); ++i)
    {
        delete _timerCommands[i];
    }
}

///////////////////////////////////////////////////////////////////////////

void BaseTimerCommandList::StartTimerCommands()
{
    for (int i = 0; i < _timerCommands.size(); ++i)
    {
        _timerCommands[i]->Start();
    }
}

///////////////////////////////////////////////////////////////////////////
