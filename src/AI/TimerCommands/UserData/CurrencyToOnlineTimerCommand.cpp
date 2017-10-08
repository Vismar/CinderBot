/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CurrencyToOnlineTimerCommand.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/UserData/RealTimeUserData.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"

using namespace Utils::Configuration;
using namespace TimerCommand::UserDataTimerCmd;
using namespace Utils::Database;

#define DEFAULT_TIMER_VALUE    60000
#define DEFAULT_CURRENCY_VALUE "1"

///////////////////////////////////////////////////////////////////////////

CurrencyToOnlineTimerCommand::CurrencyToOnlineTimerCommand()
{
    connect(&ConfigurationManager::Instance(), &ConfigurationManager::ParameterChanged,
            this, &CurrencyToOnlineTimerCommand::OnCfgParamChanged);
}

///////////////////////////////////////////////////////////////////////////

void CurrencyToOnlineTimerCommand::OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam)
{
    if (cfgParam == CfgParam::CurrencyTimer)
    {
        _UpdateTimer();
    }
}

///////////////////////////////////////////////////////////////////////////

void CurrencyToOnlineTimerCommand::_UpdateTimer()
{
    QString param;
    ConfigurationManager::Instance().GetStringParam(CfgParam::CurrencyTimer, param);
    int timerValue = param.toInt();
    if (timerValue <= 0)
    {
        timerValue = DEFAULT_TIMER_VALUE;
    }
    _timer.start(timerValue);
}

///////////////////////////////////////////////////////////////////////////

void CurrencyToOnlineTimerCommand::_TimerAction()
{
    // Set value of currency to give
    QString currencyToGive = DEFAULT_CURRENCY_VALUE;
    ConfigurationManager::Instance().GetStringParam(CfgParam::CurrencyOverTime, currencyToGive);

    const QStringList &userList = RealTimeUserData::Instance()->GetUserList();
    for (int i = 0; i < userList.count(); ++i)
    {
        QString ignoreList;
        ConfigurationManager::Instance().GetStringParam(CfgParam::IgnoreList, ignoreList);
        if (!ignoreList.contains(userList[i]))
        {
            // Get currency new value
            QString currencyValue = QString::number(UserDataDBHelper::GetUserParameter(UserDataParameter::Currency, userList[i]).toInt());
            int currencyNewValue = currencyValue.toInt() + currencyToGive.toInt();
            if (currencyNewValue < 0)
            {
                currencyNewValue  = 0;
            }

            // Update value
            currencyValue = QString::number(currencyNewValue);

            // Set new value
            UserDataDBHelper::UpdateUserParameter(UserDataParameter::Currency, currencyValue, userList[i]);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
