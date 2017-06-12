/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "InfoCovenantCommand.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

#define MSG_BASE_INFO    0
#define MSG_DESCRIPTION  1
#define MSG_NOT_PROVIDED 2

///////////////////////////////////////////////////////////////////////////

InfoCovenantCommand::InfoCovenantCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void InfoCovenantCommand::Initialize()
{
    _name = "!cov_info";
    _answers.push_back("Covenant: COV_NAME; Leader: COV_LEADER; "
                       "Members: COV_MEMBERS; Commands: COV_CMD_NUM; "
                       "Level: COV_LEVEL; Experience: COV_EXP/COV_NEED_EXP;");
    _answers.push_back("Description: ");
    _answers.push_back("Please provide name of the covenant, @.");
}

///////////////////////////////////////////////////////////////////////////

void InfoCovenantCommand::_GetAnswer(const ChatMessage &message, QStringList &answer)
{
    // Get covenant name
    QString covenant = message.GetMessage().right(message.GetMessage().size() - _name.size() - 1);
    //int covNameLength = ((covenant.size()-2) < 0) ? 0 : (covenant.size()-2);
    //covenant = covenant.left(covNameLength);
    // If user provided covenant name, try to find that covenant
    if (!covenant.isEmpty())
    {
        DB_QUERY_PTR query = DB_SELECT("Covenants", "*", QString("Name = '%1'").arg(covenant));
        if (query != NULL)
        {
            if (query->first())
            {
                QString temp = _answers.at(MSG_BASE_INFO);
                // Covenant name
                temp.replace("COV_NAME", covenant);
                // Leader of covenant
                temp.replace("COV_LEADER", query->value("Leader").toString());
                // Number of members
                DB_QUERY_PTR memberQuery = DB_SELECT("UserData", "COUNT(*)",
                                                                 QString("Covenant = '%1'").arg(covenant));
                if (memberQuery != NULL)
                {
                    if (memberQuery->first())
                    {
                        temp.replace("COV_MEMBERS", (memberQuery->value(0).toString()));
                    }
                }
                // Number of available commands
                temp.replace("COV_CMD_NUM", query->value("CmdSlots").toString());
                // Covenant level
                temp.replace("COV_LEVEL", query->value("Level").toString());
                // Covenant current exp
                temp.replace("COV_EXP", query->value("Exp").toString());
                // Covenant needed exp
                temp.replace("COV_NEED_EXP", QString::number(query->value("Level").toInt()*1000));

                // Add result to answer
                answer.append(temp);
                // Get description
                temp = query->value("Description").toString();
                if (!temp.isEmpty())
                {
                    answer.push_back(_answers.at(MSG_DESCRIPTION)+temp);
                }
            }
        }
    }
    else
    {
        answer.append(_answers.at(MSG_NOT_PROVIDED));
    }
}

///////////////////////////////////////////////////////////////////////////

void InfoCovenantCommand::_GetRandomAnswer(const ChatMessage &message, QStringList &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
