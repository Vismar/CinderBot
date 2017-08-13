/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MembersCovenantCommand.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include <QRegularExpressionMatch>
#include <QStringList>

using namespace Command::CovenantCmd;
using namespace Utils::Database;

#define MSG_MEMBERS    0
#define MSG_NOT_IN_COV 1
#define MSG_WRONG_COV  2

///////////////////////////////////////////////////////////////////////////

MembersCovenantCommand::MembersCovenantCommand()
{
    _covNameExpression.setPattern("!cov_members (?<covenant>.*)");
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void MembersCovenantCommand::Initialize()
{
    _name = "!cov_members";
    _answers.push_back("Members of 'COV_NAME': ");
    _answers.push_back("@, you are not in covenant yet. Please specify covenant name.");
    _answers.push_back("@, covenant with such name does not exist.");
}

///////////////////////////////////////////////////////////////////////////

void MembersCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    // Try to find the specified covenant
    QRegularExpressionMatch match = _covNameExpression.match(message.GetMessage());
    QString covenant;
    if (match.hasMatch())
    {
        // Save covenant if it was specified
        covenant = match.captured("covenant");
    }
    else
    {
        // If covenant was not specified, get users covenant
        DB_QUERY_PTR userQuery = DB_SELECT("UserData", "Covenant", QString("Name='%1'").arg(message.GetRealName()));
        if ((userQuery != nullptr) && (userQuery->first()))
        {
            covenant = userQuery->value("Covenant").toString();
        }
    }

    // If viewer not on covenant, notify him about it
    if (covenant == "Viewer")
    {
        answer.AddAnswer(_answers.at(MSG_NOT_IN_COV));
    }
    else
    {
        // Get all members of certain covenant
        DB_QUERY_PTR covQuery = DB_SELECT("UserData", "Author", QString("Covenant='%1'").arg(covenant));
        if (covQuery != nullptr)
        {
            // Get all members
            QStringList covMembers;
            while (covQuery->next())
            {
                covMembers.append(covQuery->value("Author").toString());
            }

            // If no members of specified covenant, that is mean that such covenant doesn't exist
            if (covMembers.isEmpty())
            {
                answer.AddAnswer(_answers.at(MSG_WRONG_COV));
            }
            else
            {
                QString answerStr = _answers.at(MSG_MEMBERS);

                // Append all members of covenant to string
                for (int i = 0; i < covMembers.size(); ++i)
                {
                    // If number of members in specified covenant is too big,
                    // split into spearate messages that will be sent to chat
                    if ((answerStr.size() + covMembers.at(i).size()) >= 498)
                    {
                        answer.AddAnswer(answerStr);
                        answerStr.clear();
                    }
                    answerStr.append(covMembers.at(i));
                    // If this member was not the last, add ", "
                    if (i < (covMembers.size() - 1))
                    {
                        answerStr.append(", ");
                    }
                    // If it was the last member, add "."
                    else
                    {
                        answerStr.append(".");
                    }
                }

                // Add last message to answer
                if (!answerStr.isEmpty())
                {
                    answer.AddAnswer(answerStr);
                }

                // Replace cov name in first message
                (*answer.GetAnswers().begin()).replace("COV_NAME", covenant);
            }
        }
        // If query return nullptr somehowm display that such covenant is not exist
        else
        {
            answer.AddAnswer(_answers.at(MSG_WRONG_COV));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void MembersCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
