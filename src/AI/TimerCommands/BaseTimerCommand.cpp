/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "BaseTimerCommand.hpp"

using namespace TimerCommand;

///////////////////////////////////////////////////////////////////////////

BaseTimerCommand::BaseTimerCommand() : QObject(0) { }

///////////////////////////////////////////////////////////////////////////

BaseTimerCommand::~BaseTimerCommand() { }

///////////////////////////////////////////////////////////////////////////

void BaseTimerCommand::Start()
{
    connect(&_timer, &QTimer::timeout,
            this, &BaseTimerCommand::_OnTimerEnds);
    _UpdateTimer();
}

///////////////////////////////////////////////////////////////////////////

void BaseTimerCommand::_OnTimerEnds()
{
    _TimerAction();
    _UpdateTimer();
}

///////////////////////////////////////////////////////////////////////////
