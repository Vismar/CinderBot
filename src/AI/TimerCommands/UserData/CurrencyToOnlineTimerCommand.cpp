/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CurrencyToOnlineTimerCommand.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Twitch/KrakenClient.hpp"

using namespace Utils::Configuration;
using namespace TimerCommand::UserDataTimerCmd;
using namespace Utils::Database;
using namespace Twitch;

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
    // Get tiemr value from configuration manager
    QString param;
    ConfigurationManager::Instance().GetStringParam(CfgParam::CurrencyTimer, param);
    int timerValue = param.toInt();

    // Check value
    if (timerValue <= 0)
    {
        timerValue = DEFAULT_TIMER_VALUE;
    }

    // Start timer anew
    _timer.start(timerValue);
}

///////////////////////////////////////////////////////////////////////////

void CurrencyToOnlineTimerCommand::_TimerAction()
{
    // Only if stream is on we should reward user with currency
    if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
    {
        // Set value of currency to give
        QString currencyToGive = DEFAULT_CURRENCY_VALUE;
        ConfigurationManager::Instance().GetStringParam(CfgParam::CurrencyOverTime, currencyToGive);

        UserDataDBHelper::GiveCurrencyToOnlineUsers(currencyToGive.toInt());
    }
}

///////////////////////////////////////////////////////////////////////////
