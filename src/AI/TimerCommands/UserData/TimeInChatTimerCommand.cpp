/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "TimeInChatTimerCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Twitch/KrakenClient.hpp"

using namespace Utils::Configuration;
using namespace TimerCommand::UserDataTimerCmd;
using namespace Utils::Database;
using namespace Twitch;

#define MINUTE 60000

///////////////////////////////////////////////////////////////////////////

void TimeInChatTimerCommand::_UpdateTimer()
{
    _timer.start(MINUTE);
}

///////////////////////////////////////////////////////////////////////////

void TimeInChatTimerCommand::_TimerAction()
{
    // Only if stream is on we should update time counter
    if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
    {
        UserDataDBHelper::UpdateTimeInChat(1);
        
    }
}

///////////////////////////////////////////////////////////////////////////
