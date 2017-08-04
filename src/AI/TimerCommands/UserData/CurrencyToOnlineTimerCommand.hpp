/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/TimerCommands/BaseTimerCommand.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

/*!
 * \brief Contains all timer command related.
 */
namespace TimerCommand
{
/*!
 * \brief Contains all user data timer commands.
 */
namespace UserDataTimerCmd
{

/*!
 * \brief Timer command to give currency on timer.
 *
 * Gives specified in configuration manager amount of currency to all users in chat.
 */
class CurrencyToOnlineTimerCommand : public BaseTimerCommand
{
    Q_OBJECT
public:
    CurrencyToOnlineTimerCommand();

public slots:
    /*!
     * \brief Updates timer if cfgParam == CurrencyTimer.
     * \param cfgParam - configuration paramter.
     *
     * Get new value of CurrencyTimer and use it to restart timer.
     */
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam);

protected:
    ////////////////////////////////
    /// BaseTimerCommand override
    void _UpdateTimer();
    void _TimerAction();
};

}
}
