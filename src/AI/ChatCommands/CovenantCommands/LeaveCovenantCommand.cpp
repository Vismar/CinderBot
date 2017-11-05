/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "LeaveCovenantCommand.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

enum
{
    MsgLeftCov = 0,
    MsgUserLeader,
    MsgLeftAndNewLeader,
    MsgSpecifieNewLeader,
    MsgNotInCovenant
};

///////////////////////////////////////////////////////////////////////////

LeaveCovenantCommand::LeaveCovenantCommand()
{
    _name = "!cov_leave";
    _answers.push_back("You left covenant! Now you are just a viewer, @");
    _answers.push_back("You are leader of covenant, @! Please provide name of the new leader.");
    _answers.push_back("You left covenant, @! New leader of 'COV_NAME' is LEADER_NAME.");
    _answers.push_back("Specified user is not member of your covenant!");
    _answers.push_back("You are not in any covenant yet, @.");
}

///////////////////////////////////////////////////////////////////////////

void LeaveCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    QString covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        if (CovenantDBHelper::CheckLeadership(message.GetUserID()))
        {
            int startOfName = message.GetMessage().indexOf(" ");
            if (startOfName > 0)
            {
                // Try to get name of new leader
                QString newLeader = message.GetMessage().right(message.GetMessage().size() - startOfName - 1);

                // If leader name was found, try to use it
                if (!newLeader.isEmpty())
                {
                    // Check if user provided his own name
                    if ((newLeader == message.GetRealName()) || (newLeader == message.GetAuthor()))
                    {
                        answer.AddAnswer(_answers.at(MsgUserLeader));
                    }

                    // If user provided different name
                    if (answer.GetAnswers().isEmpty())
                    {
                        if (_SetNewLeaderToCovenant(newLeader, message.GetUserID(), covenant))
                        {
                            // Add answer and replace placeholders
                            answer.AddAnswer(_answers.at(MsgLeftAndNewLeader));
                            (*answer.GetAnswers().begin()).replace("COV_NAME", covenant);
                            (*answer.GetAnswers().begin()).replace("LEADER_NAME", newLeader);
                        }
                        // Notify that user is not member of covenant
                        else
                        {
                            answer.AddAnswer(_answers.at(MsgSpecifieNewLeader));
                        }
                    }
                }
                // Notify that name was not specified
                else
                {
                    answer.AddAnswer(_answers.at(MsgUserLeader));
                }
            }
            // If message do not contain ampersand, so that means user do not provide name of new leader
            else
            {
                answer.AddAnswer(_answers.at(MsgUserLeader));
            }
        }
        // If user is not the leader of covenant, that just change its state
        else
        {
            UserDataDBHelper::UpdateUserParameter(UserDataParameter::Covenant, "Viewer", message.GetUserID());
            answer.AddAnswer(_answers.at(MsgLeftCov));
        }
    }
    // If user is not in any covenant
    else
    {
        answer.AddAnswer(_answers.at(MsgNotInCovenant));
    }
}

///////////////////////////////////////////////////////////////////////////

void LeaveCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////

bool LeaveCovenantCommand::_SetNewLeaderToCovenant(const QString &newLeader, int oldLeaderID, const QString &covName) const
{
    bool result(false);

    // Check if provided user is member of covenant
    if (UserDataDBHelper::IsUserInCovenant(covName, newLeader))
    {
        // Change covenant of user
        UserDataDBHelper::UpdateUserParameter(UserDataParameter::Covenant, "Viewer", oldLeaderID);
        
        // Get params of a new leader
        UserDataParams userParams = UserDataDBHelper::GetUserParameters(newLeader);

        // Update leader name of covenant
        result = CovenantDBHelper::SetParameter(CovenantParameter::Leader, userParams.Name, userParams.Covenant);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
