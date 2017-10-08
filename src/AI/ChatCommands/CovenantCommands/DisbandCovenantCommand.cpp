/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "DisbandCovenantCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

#define MSG_DISBANDING  0
#define MSG_NOT_LEADER  1
#define MSG_NO_COVENANT 2

///////////////////////////////////////////////////////////////////////////

DisbandCovenantCommand::DisbandCovenantCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void DisbandCovenantCommand::Initialize()
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
        DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
        if (query != nullptr)
        {
            query->first();
            // If user is leader, than proceed disband of covenant
            if (query->value("Leader").toString() == message.GetRealName())
            {
                // If covenant was disbanded, set covenant field to viewer for all users who was in that covenant
                if (DB_DELETE("Covenants", QString("Name = '%1'").arg(covenant)))
                {
                    // Update covenant field for all users who was in covenant
                    UserDataDBHelper::UpdateCovenantName(covenant, "Viewer");
                    answer.AddAnswer(_answers.at(MSG_DISBANDING));

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
                answer.AddAnswer(_answers.at(MSG_NOT_LEADER));
            }
        }
    }
    // If user is not a member of any covenant
    else
    {
        answer.AddAnswer(_answers.at(MSG_NO_COVENANT));
    }
}

///////////////////////////////////////////////////////////////////////////

void DisbandCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
