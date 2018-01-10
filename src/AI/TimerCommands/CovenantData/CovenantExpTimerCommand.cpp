/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CovenantExpTimerCommand.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Twitch/KrakenClient.hpp"
#include <QtMath>

using namespace TimerCommand::CovenantDataTimerCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;
using namespace Twitch;

#define DEFAULT_TIMER_VALUE 300000
#define DEFAULT_EXP_VALUE   10

///////////////////////////////////////////////////////////////////////////

CovenantExpTimerCommand::CovenantExpTimerCommand()
{
    connect(&ConfigurationManager::Instance(), &ConfigurationManager::ParameterChanged,
            this, &CovenantExpTimerCommand::OnCfgParamChanged);
}

///////////////////////////////////////////////////////////////////////////

void CovenantExpTimerCommand::OnCfgParamChanged(CfgParam cfgParam)
{
    switch(cfgParam)
    {
    case CfgParam::CovExpTimer:
        _UpdateTimer();
        break;
    case CfgParam::CovExpToLvl:
    case CfgParam::CovMembersDefault:
    case CfgParam::CovMembersPerUp:
    case CfgParam::CovMembersLvlRequirement:
    case CfgParam::CovCmdSlotsDefault:
    case CfgParam::CovCmdSlotsPerUp:
    case CfgParam::CovCmdSlotsLvlRequirement:
        _UpdateCovenants(0);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

void CovenantExpTimerCommand::_UpdateTimer()
{
    // Get timer value from configuration manager
    QString paramValue;
    ConfigurationManager::Instance().GetStringParam(CfgParam::CovExpTimer, paramValue);
    int timerValue = paramValue.toInt();

    // Check value
    if (timerValue <= 0)
    {
        timerValue = DEFAULT_TIMER_VALUE;
    }

    // Start timer anew
    _timer.start(timerValue);
}

///////////////////////////////////////////////////////////////////////////

void CovenantExpTimerCommand::_TimerAction()
{
    // Only if stream is on covenants should gain exp
    if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
    {
        // Set exp value
        QString exp;
        ConfigurationManager::Instance().GetStringParam(CfgParam::CovExpGain, exp);
        int expGain = exp.toInt();
        if (expGain <= 0)
        {
            expGain = DEFAULT_EXP_VALUE;
        }
        
        _UpdateCovenants(expGain);        
    }
}

///////////////////////////////////////////////////////////////////////////

void CovenantExpTimerCommand::_UpdateCovenants(int expGain) const
{
    // Start transaction
    if (DatabaseManager::Instance().StartTransaction())
    {
        // Get covenant list
        QStringList covenantList = CovenantDBHelper::GetCovenants();

        // Check members for every covenant
        for (QString covenant : covenantList)
        {
            // Get covenant members list
            QStringList covenantMemberList = UserDataDBHelper::GetUsersFromCovenant(covenant);
            int numberOfCovenantMembers = 0;

            // Count how many members of covenant currently in chat
            for (QString covenantMember : covenantMemberList)
            {
                if (UserDataDBHelper::IsUserInChat(covenantMember))
                {
                    ++numberOfCovenantMembers;
                }
            }

            // If at least 1 member of covenant in chat, gain exp
            if (numberOfCovenantMembers > 0)
            {
                qreal scale = qSqrt(qSqrt(numberOfCovenantMembers));
                CovenantDBHelper::GainExpToCovenant(expGain * scale, covenant);
                _CheckLevel(covenant);
            }
        }

        // End transaction
        DatabaseManager::Instance().EndTransaction();
    }
}

///////////////////////////////////////////////////////////////////////////

void CovenantExpTimerCommand::_CheckLevel(const QString &covenantName) const
{
    QString covExpToLvl;
    ConfigurationManager &cfgManager = ConfigurationManager::Instance();
    if (cfgManager.GetStringParam(CfgParam::CovExpToLvl, covExpToLvl))
    {        
        CovParams covParams = CovenantDBHelper::GetParams(covenantName);
        int requiredExpToLvl = covExpToLvl.toInt() * covParams.Level;        

        // Update level
        while (covParams.Exp >= requiredExpToLvl)
        {
            ++covParams.Level;
            covParams.Exp -= requiredExpToLvl;
            requiredExpToLvl = covExpToLvl.toInt() * covParams.Level;
        }

        // Update covenant params
        _UpdateMembers(covParams);
        _UpdateCmdSlots(covParams);

        // Set new params
        CovenantDBHelper::SetParams(covParams);
    }
}

///////////////////////////////////////////////////////////////////////////

void CovenantExpTimerCommand::_UpdateMembers(CovParams &covParams) const
{
    ConfigurationManager &cfgManager = ConfigurationManager::Instance();

    // Updating members
    QString covMembersDefault;
    QString covMembersPerUp;
    QString covMembersLvlRequirement;
    if (cfgManager.GetStringParam(CfgParam::CovMembersDefault, covMembersDefault) &&
        cfgManager.GetStringParam(CfgParam::CovMembersPerUp, covMembersPerUp) &&
        cfgManager.GetStringParam(CfgParam::CovMembersLvlRequirement, covMembersLvlRequirement))
    {
        int memberUps = (covParams.Level - 1) / covMembersLvlRequirement.toInt();
        covParams.MaxMembers = covMembersDefault.toInt() + covMembersPerUp.toInt() * memberUps;
    }
}

///////////////////////////////////////////////////////////////////////////

void CovenantExpTimerCommand::_UpdateCmdSlots(CovParams &covParams) const
{
    ConfigurationManager &cfgManager = ConfigurationManager::Instance();

    // Updating cmd slots
    QString covCmdSlotsDefault;
    QString covCmdSlotsPerUp;
    QString covCmdSlotsLvlRequirement;
    if (cfgManager.GetStringParam(CfgParam::CovCmdSlotsDefault, covCmdSlotsDefault) &&
        cfgManager.GetStringParam(CfgParam::CovCmdSlotsPerUp, covCmdSlotsPerUp) &&
        cfgManager.GetStringParam(CfgParam::CovCmdSlotsLvlRequirement, covCmdSlotsLvlRequirement))
    {
        int cmdUps = (covParams.Level - 1) / covCmdSlotsLvlRequirement.toInt();
        covParams.CmdSlots = covCmdSlotsDefault.toInt() + covCmdSlotsPerUp.toInt() * cmdUps;
    }
}

///////////////////////////////////////////////////////////////////////////
