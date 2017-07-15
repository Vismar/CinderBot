/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "DescriptionCovenantCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command::CovenantCmd;

#define MSG_DESCRIPTION_CHANGED 0
#define MSG_USER_NOT_LEADER     1
#define MSG_NOT_IN_COVENANT     2

///////////////////////////////////////////////////////////////////////////

DescriptionCovenantCommand::DescriptionCovenantCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void DescriptionCovenantCommand::Initialize()
{
    _name = "!cov_description";
    _answers.push_back("Covenant description was changed, @!");
    _answers.push_back("You are not leader of your covenant, @!");
    _answers.push_back("You are not in any covenant yet, @.");
}

///////////////////////////////////////////////////////////////////////////

void DescriptionCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    QString covenant = UD_GET_PARAM(message.GetRealName(), UDP_Covenant);
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
        if (query->exec())
        {
            if (query->first())
            {
                // If user leader of its covenant, try to get description
                if (query->value("Leader").toString() == message.GetRealName())
                {
                    // If description not provided, just make it empty
                    if (message.GetMessage().size() <= _name.size() + 2)
                    {
                        DB_UPDATE("Covenants", "Description = ''", QString("Name = '%1'").arg(covenant));
                        answer.AddAnswer(_answers.at(MSG_DESCRIPTION_CHANGED));
                    }
                    // User provided something, get it and save as description
                    else
                    {
                        int lengthOfDescription = message.GetMessage().size()-_name.size()-1;
                        // Description of covenant should not contain more than 400 symbols
                        if (lengthOfDescription > 400)
                        {
                            lengthOfDescription = 400;
                        }
                        QString description = message.GetMessage().mid(_name.size()+1, lengthOfDescription);
                        description.replace("'", "");
                        DB_UPDATE("Covenants", QString("Description = '%1'").arg(description),
                                               QString("Name = '%1'").arg(covenant));
                        answer.AddAnswer(_answers.at(MSG_DESCRIPTION_CHANGED));
                    }
                }
                // If user not leader of its covenant, notify
                else
                {
                    answer.AddAnswer(_answers.at(MSG_USER_NOT_LEADER));
                }
            }
            // If user is a leader, he is allowed to create/edit covenant description
            if (answer.GetAnswers().isEmpty())
            {
            }
        }
    }
    // If user not in covenant, notify
    else
    {
        answer.AddAnswer(_answers.at(MSG_NOT_IN_COVENANT));
    }
}

///////////////////////////////////////////////////////////////////////////

void DescriptionCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
