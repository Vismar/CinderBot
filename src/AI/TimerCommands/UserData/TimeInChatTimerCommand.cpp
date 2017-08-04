/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "TimeInChatTimerCommand.hpp"
#include "Utils/UserData/RealTimeUserData.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/Config/ConfigurationManager.hpp"

using namespace Utils::Configuration;
using namespace TimerCommand::UserDataTimerCmd;

#define MINUTE 60000

///////////////////////////////////////////////////////////////////////////

void TimeInChatTimerCommand::_UpdateTimer()
{
    _timer.start(MINUTE);
}

///////////////////////////////////////////////////////////////////////////

void TimeInChatTimerCommand::_TimerAction()
{
    const QStringList &userList = RealTimeUserData::Instance()->GetUserList();
    for (int i = 0; i < userList.count(); ++i)
    {
        QString ignoreList;
        ConfigurationManager::Instance().GetStringParam(IgnoreList, ignoreList);
        if (!ignoreList.contains(userList[i]))
        {
            // Get time in chat
            QString timeValue = UD_GET_PARAM(userList[i], UDP_TimeInChat);
            int timeNewValue = timeValue.toInt() + 1;
            if (timeNewValue < 0)
            {
                timeNewValue = 0;
            }

            // Update value
            timeValue = QString::number(timeNewValue);

            // Set value
            UD_UPDATE(userList[i], UDP_TimeInChat, timeValue);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
