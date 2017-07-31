/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QVector>
#include "AI/TimerCommands/BaseTimerCommand.hpp"

/*!
 * \brief Contains all timer command related.
 */
namespace TimerCommand
{

/*!
 * \brief Base timer commands list.
 *
 * Contains specific timer commands to store, start and delete them.
 */
class BaseTimerCommandList : public QObject
{
    Q_OBJECT
public:
    BaseTimerCommandList();
    virtual ~BaseTimerCommandList();

    /*!
     * \brief Starts all timer commands in the list.
     *
     * Picks one timer command after another and starts its timer.
     */
    virtual void StartTimerCommands();

protected:
    /*! List of timer commands */
    QVector<BaseTimerCommand*> _timerCommands;
};

}
