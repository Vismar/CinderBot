/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "LeaveCovenantCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

#define MSG_LEFT_COV            0
#define MSG_USER_LEADER         1
#define MSG_LEFT_AND_NEW_LEADER 2
#define MSG_SPECIFIE_NEW_LEADER 3
#define MSG_NOT_IN_COVENANT     4

///////////////////////////////////////////////////////////////////////////

LeaveCovenantCommand::LeaveCovenantCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void LeaveCovenantCommand::Initialize()
{
    _name = "!cov_leave";
    _answers.push_back("You left covenant! Now you are just a viewer, @");
    _answers.push_back("You are leader of covenant, @! Please provide name of the new leader.");
    _answers.push_back("You left covenant, @! New leader of COV_NAME is LEADER_NAME.");
    _answers.push_back("Specified user is not member of your covenant!");
    _answers.push_back("You are not in any covenant yet, @.");
}

///////////////////////////////////////////////////////////////////////////

void LeaveCovenantCommand::_GetAnswer(const ChatMessage &message, QStringList &answer)
{
    QString covenant = UD_GET_PARAM(message.GetRealName(), UDP_Covenant);
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
        if (query->exec())
        {
            query->first();
            // If user is leader, than try to find specified name of new leader
            if (query->value("Leader").toString() == message.GetRealName())
            {
                int startOfName = message.GetMessage().indexOf(" ");
                if (startOfName > 0)
                {
                    // Try to get name of new leader
                    QString newLeader;
                    newLeader = message.GetMessage().right(message.GetMessage().size() - startOfName - 1);
                    // If leader name was found, try to use it
                    if (!newLeader.isEmpty())
                    {
                        // Check if user provided his own name
                        if (newLeader.toLower() == message.GetRealName())
                        {
                            answer.append(_answers.at(MSG_USER_LEADER));
                        }
                        // If user provided different name
                        if (answer.isEmpty())
                        {
                            if (_SetNewLeaderToCovenant(newLeader, message.GetRealName(), covenant))
                            {
                                answer.append(_answers.at(MSG_LEFT_AND_NEW_LEADER));
                                (*answer.begin()).replace("COV_NAME", covenant);
                                (*answer.begin()).replace("LEADER_NAME", newLeader);
                            }
                            // Notify that user is not member of covenant
                            else
                            {
                                answer.append(_answers.at(MSG_SPECIFIE_NEW_LEADER));
                            }
                        }
                    }
                    // Notify that name was not specified
                    else
                    {
                        answer.append(_answers.at(MSG_USER_LEADER));
                    }
                }
                // If message do not contain ampersand, so that means user do not provide name of new leader
                else
                {
                    answer.append(_answers.at(MSG_USER_LEADER));
                }
            }
            // If user is not a leader of covenant, that just change its state
            else
            {
                UD_UPDATE(message.GetRealName(), UDP_Covenant, "Viewer");
                answer.append(_answers.at(MSG_LEFT_COV));
            }
        }
    }
    // If user is not in any covenant
    else
    {
        answer.append(_answers.at(MSG_NOT_IN_COVENANT));
    }
}

///////////////////////////////////////////////////////////////////////////

void LeaveCovenantCommand::_GetRandomAnswer(const ChatMessage &message, QStringList &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////

bool LeaveCovenantCommand::_SetNewLeaderToCovenant(const QString &newLeader, const QString &oldLeader, const QString &covName)
{
    bool result(false);
    // Check if provided user is member of covenant
    DB_QUERY_PTR newLeaderQuery = DB_SELECT("UserData", "Covenant", QString("Name = '%1'").arg(newLeader));
    if (newLeaderQuery->exec())
    {
        if (newLeaderQuery->first())
        {
            // If so, make him leader
            if (newLeaderQuery->value("Covenant").toString() == covName)
            {
                // Change covenant of user
                UD_UPDATE(oldLeader, UDP_Covenant, "Viewer");
                // Update leader name of covenant
                DB_UPDATE("Covenants", QString("Leader = '%1'").arg(newLeader),
                                       QString("Name = '%1'").arg(covName));
                result = true;
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
