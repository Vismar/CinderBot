/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/TimerCommands/BaseTimerCommandList.hpp"

/*!
* \brief Contains all timer command related.
*/
namespace TimerCommand
{
/*!
 * \brief timer commands related to covenant data.
 */
namespace CovenantDataTimerCmd
{

class CovenantDataTimerCommandList : public BaseTimerCommandList
{
public:
    CovenantDataTimerCommandList();
};

}
}