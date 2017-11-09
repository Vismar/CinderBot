/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/TimerCommands/BaseTimerCommand.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

// Forward declaration
namespace Utils 
{
namespace Database 
{

struct CovParams;

}
}

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
   
/*!
 * \brief Timer command to gain exp to covenants.
 */
class CovenantExpTimerCommand : public BaseTimerCommand
{
    Q_OBJECT
public:
    CovenantExpTimerCommand();

public slots:
    /*!
     * \brief Updates timer if cfgParam == CovExpTimer.
     * \param cfgParam - configuration paramter.
     * 
     * Get new value of CovExpTimer nad use it to restart timer.
     */
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam);

protected:
    /*///////////////////////////////*/
    /* BaseTimerCommand overrides    */
    /*///////////////////////////////*/
    /*!
     * \brief Starts timer anew.
     */
    void _UpdateTimer() override;
    /*!
     * \brief Check how many covenant members currently in chat and gains expirience to covenant if stream is live.
     */
    void _TimerAction() override;

private:
    /*!
     * \brief Grabs covenant list and updates every one of them.
     * \param expGain - amount of exp that will be given to covenants.
     */
    void _UpdateCovenants(int expGain) const;
    /*!
     * \brief Check if covenant has new level and updates covenant parameters.
     * \param covenantName - name of certain covenant to check and update.
     */
    void _CheckLevel(const QString &covenantName) const;
    /*!
     * \brief Updates max number of members that available for certain covenant.
     * \param covParams - parameters of certain covenant.
     */
    void _UpdateMembers(Utils::Database::CovParams &covParams) const;
    /*!
     * \brief Updates max number of cmd slots  that available for certain covenant.
     * \param covParams - parameters of certain covenant.
     */
    void _UpdateCmdSlots(Utils::Database::CovParams &covParams) const;
};

}
}