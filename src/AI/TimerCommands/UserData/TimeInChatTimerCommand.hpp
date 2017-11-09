/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/TimerCommands/BaseTimerCommand.hpp"

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
 * \brief Updates time that was spent by users in chat.
 *
 * Updates time that was spent by users in chat. Stores it in minutes.
 */
class TimeInChatTimerCommand : public BaseTimerCommand
{
protected:
    /*///////////////////////////////*/
    /* BaseTimerCommand overrides    */
    /*///////////////////////////////*/
    /*!
     * \brief Starts timer anew.
     */
    void _UpdateTimer() override;
    /*!
     * \brief Updates timer counter for all users in chat if stream is live.
     */
    void _TimerAction() override;
};

}
}
