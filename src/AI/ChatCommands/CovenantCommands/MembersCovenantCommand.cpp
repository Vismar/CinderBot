/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MembersCovenantCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include <QRegularExpressionMatch>
#include <QStringList>

using namespace Command::CovenantCmd;
using namespace Utils::Database;

enum
{
    MsgMembers = 0,
    MsgNotInCov,
    MsgWrongCov
};

///////////////////////////////////////////////////////////////////////////

MembersCovenantCommand::MembersCovenantCommand()
{
    _covNameExpression.setPattern("!cov_members (?<covenant>.*)");
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
        covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();
    }

    // If viewer not on covenant, notify him about it
    if (covenant == "Viewer")
    {
        answer.AddAnswer(_answers.at(MsgNotInCov));
    }
    else
    {
        // Get all members of certain covenant
        QStringList covMembers = UserDataDBHelper::GetAuthorsFromCovenant(covenant);

        // If no members of specified covenant, that is mean that such covenant doesn't exist
        if (covMembers.isEmpty())
        {
            answer.AddAnswer(_answers.at(MsgWrongCov));
        }
        else
        {
            QString answerStr = _answers.at(MsgMembers);

            // Append all members of covenant to string
            for (int i = 0; i < covMembers.size(); ++i)
            {
                // If number of members in specified covenant is too big,
                // split into separate messages that will be sent to chat
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
}

///////////////////////////////////////////////////////////////////////////

void MembersCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
