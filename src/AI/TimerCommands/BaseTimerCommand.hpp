/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include <QTimer>

/*!
 * \brief Contains all timer command related.
 */
namespace TimerCommand
{

/*!
 * \brief Base class for all commands that executes on timer.
 *
 * Timer commands executes itself on a timer and produce certain
 * actions that should be defined in inherited classes..
 */
class BaseTimerCommand : public QObject
{
    Q_OBJECT
public:
    BaseTimerCommand();
    virtual ~BaseTimerCommand();

    /*!
     * \brief Start timer command.
     *
     * Connects timer event and _OnTimerEnds slot. Starts timer execution.
     */
    void Start();

protected:
    /*!
     * \brief Updates timer value and starts it.
     *
     * Sets new value for timer and start it. Should be implemented by inherited class.
     */
    virtual void _UpdateTimer() = 0;
    /*!
     * \brief Action to perform on timer event.
     *
     * Performs certain action on timer event. Should be implemented by inherited class.
     */
    virtual void _TimerAction() = 0;

private slots:
    /*!
     * \brief Executes command on a timer and starts timer anew.
     *
     * Calls _TimerAction to perform certain action and start timer anew through UpdateTimer.
     */
    void _OnTimerEnds();

protected:
    /*! Timer */
    QTimer _timer;
};

}
