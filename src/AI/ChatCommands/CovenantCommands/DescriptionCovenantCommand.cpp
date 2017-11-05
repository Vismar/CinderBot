/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "DescriptionCovenantCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

enum
{
    MsgDescriptionChanged = 0,
    MsgIserNotLeader,
    MsgNotInCovenant
};

///////////////////////////////////////////////////////////////////////////

DescriptionCovenantCommand::DescriptionCovenantCommand()
{
    _name = "!cov_description";
    _answers.push_back("Covenant description was changed, @!");
    _answers.push_back("You are not leader of your covenant, @!");
    _answers.push_back("You are not in any covenant yet, @.");
}

///////////////////////////////////////////////////////////////////////////

void DescriptionCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    QString covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        if (CovenantDBHelper::CheckLeadership(message.GetUserID()))
        {

            // If description not provided, just make it empty
            if (message.GetMessage().size() <= _name.size() + 2)
            {
                if (CovenantDBHelper::SetParameter(CovenantParameter::Description, "", covenant))
                {
                    answer.AddAnswer(_answers.at(MsgDescriptionChanged));
                }
            }
            // User provided something, get it and save as description
            else
            {
                int lengthOfDescription = message.GetMessage().size() - _name.size() - 1;
                // Description of covenant should not contain more than 400 symbols
                if (lengthOfDescription > 400)
                {
                    lengthOfDescription = 400;
                }
                QString description = message.GetMessage().mid(_name.size() + 1, lengthOfDescription);
                description.replace("'", "");
                if (CovenantDBHelper::SetParameter(CovenantParameter::Description, description, covenant))
                {
                    answer.AddAnswer(_answers.at(MsgDescriptionChanged));
                }
            }
        }
        // If user not leader of its covenant, notify
        else
        {
            answer.AddAnswer(_answers.at(MsgIserNotLeader));
        }
    }
    // If user not in covenant, notify
    else
    {
        answer.AddAnswer(_answers.at(MsgNotInCovenant));
    }
}

///////////////////////////////////////////////////////////////////////////

void DescriptionCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
