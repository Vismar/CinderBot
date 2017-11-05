/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "DisbandCovenantCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

enum
{
    MsgDisbanding = 0,
    MsgNotLeader,
    MsgNoCovenant
};

///////////////////////////////////////////////////////////////////////////

DisbandCovenantCommand::DisbandCovenantCommand()
{
    _name = "!cov_disband";
    _answers.push_back("Your covenant was disbanded, @!");
    _answers.push_back("You are not leader of your covenant, @.");
    _answers.push_back("You are not in any covenant yet, @.");
}

///////////////////////////////////////////////////////////////////////////

void DisbandCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    QString covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        if (CovenantDBHelper::CheckLeadership(message.GetUserID()))
        {
            // If covenant was disbanded, set covenant field to viewer for all users who was in that covenant
            if (CovenantDBHelper::DisbandCovenant(covenant))
            {
                // Update covenant field for all users who was in covenant
                UserDataDBHelper::UpdateCovenantName(covenant, "Viewer");
                answer.AddAnswer(_answers.at(MsgDisbanding));

                // Also we need to delete all command that were created for this covenant
                QStringList commands = CustomCommandDBHelper::Instance().GetCommandNames(CmdType::CovenantCmd, covenant);
                for (int i = 0; i < commands.size(); ++i)
                {
                    CustomCommandDBHelper::Instance().DeleteCommand(CmdType::CovenantCmd, commands.at(i));
                }
            }
        }
        // If user not leader, say it
        else
        {
            answer.AddAnswer(_answers.at(MsgNotLeader));
        }
    }
    // If user is not a member of any covenant
    else
    {
        answer.AddAnswer(_answers.at(MsgNoCovenant));
    }
}

///////////////////////////////////////////////////////////////////////////

void DisbandCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
