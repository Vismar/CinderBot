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
 * \brief Contains all user data timer commands.
 */
namespace UserDataTimerCmd
{

/*!
 * \brief User data timer command list contains all user data timer commands.
 */
class UserDataTimerCommandList : public BaseTimerCommandList
{
public:
    UserDataTimerCommandList();
    ~UserDataTimerCommandList();
};

}
}
