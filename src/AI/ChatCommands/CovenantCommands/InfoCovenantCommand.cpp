/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "InfoCovenantCommand.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Config/ConfigurationManager.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;
using namespace Utils::Configuration;

enum
{
    MsgBaseInfo = 0,
    MsgDescription,
    MsgNotProvided
};

///////////////////////////////////////////////////////////////////////////

InfoCovenantCommand::InfoCovenantCommand()
{
    _name = "!cov_info";
    _answers.push_back("Covenant: COV_NAME; Leader: COV_LEADER; "
                       "Members: COV_MEMBERS; Commands: COV_CMD_NUM/COV_CMD_SLOTS; "
                       "Level: COV_LEVEL; Experience: COV_EXP/COV_NEED_EXP;");
    _answers.push_back("Description: ");
    _answers.push_back("Please provide name of the covenant, @.");
}

///////////////////////////////////////////////////////////////////////////

void InfoCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    // Get covenant name
    QString covenant = message.GetMessage().right(message.GetMessage().size() - _name.size() - 1);

    // If user provided covenant name, try to find that covenant
    if (!covenant.isEmpty())
    {
        CovParams covParams = CovenantDBHelper::GetParams(covenant);
        // If covenant exist
        if (!covParams.Name.isEmpty())
        {
            QString temp = _answers.at(MsgBaseInfo);

            // Covenant name
            temp.replace("COV_NAME", covenant);

            // Leader of covenant
            temp.replace("COV_LEADER", covParams.Leader);

            // Number of members
            temp.replace("COV_MEMBERS", QString::number(UserDataDBHelper::GetUsersFromCovenant(covenant).size()) +
                                        QString("/%1").arg(covParams.MaxMembers));

            // Number of created commands
            int covCmdNum = CustomCommandDBHelper::Instance().GetNumberOfCommands(CmdType::CovenantCmd, covenant);
            temp.replace("COV_CMD_NUM", QString::number(covCmdNum));

            // Number of available command slots
            temp.replace("COV_CMD_SLOTS", QString::number(covParams.CmdSlots));

            // Covenant level
            temp.replace("COV_LEVEL", QString::number(covParams.Level));

            // Covenant current exp
            temp.replace("COV_EXP", QString::number(covParams.Exp));

            // Covenant needed exp
            QString covExpToLvl;
            ConfigurationManager::Instance().GetStringParam(CfgParam::CovExpToLvl, covExpToLvl);
            temp.replace("COV_NEED_EXP", QString::number(covParams.Level * covExpToLvl.toInt()));

            // Add result to answer
            answer.AddAnswer(temp);

            // Get description
            temp = covParams.Description;
            if (!temp.isEmpty())
            {
                answer.AddAnswer(_answers.at(MsgDescription) + temp);
            }
        }
    }
    else
    {
        answer.AddAnswer(_answers.at(MsgNotProvided));
    }
}

///////////////////////////////////////////////////////////////////////////

void InfoCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
