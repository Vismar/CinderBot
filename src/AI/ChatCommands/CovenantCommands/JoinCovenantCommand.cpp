/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "JoinCovenantCommand.hpp"
#include <Utils/UserData/UserData.hpp>
#include <Utils/Config/ConfigurationManager.hpp>
#include <Utils/Config/ConfigurationParameters.hpp>
#include <Utils/DatabaseManager.hpp>

using namespace Command;

#define MSG_JOINING_COV    0
#define MSG_ALREADY_IN_COV 1
#define MSG_NO_CURRENCY    2
#define MSG_USER_IS_LEADER 3

///////////////////////////////////////////////////////////////////////////

JoinCovenantCommand::JoinCovenantCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void JoinCovenantCommand::Initialize()
{
    _name = "!cov_join";
    _answers.push_back("You are joining 'COV_NAME', @.");
    _answers.push_back("You are already in that covenant, @!");
    _answers.push_back("Not enough currency, @!");
    _answers.push_back("You are leader of your covenant, @! Please leave before trying to join.");
}

///////////////////////////////////////////////////////////////////////////

QString JoinCovenantCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    if (message.GetMessage().contains(_name))
    {
        QString price;
        ConfigurationManager& configMng = ConfigurationManager::Instance();
        QString covenant = UD_GET_PARAM(message.GetRealName(), UDP_Covenant);

        // Check if user is leader of its covenant
        if (covenant != "Viewer")
        {
            DB_QUERY_PTR queryLeader = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
            if (queryLeader->exec())
            {
                queryLeader->first();
                // If user is leader, notify him about it
                if (queryLeader->value("Leader").toString() == message.GetRealName())
                {
                    answer = _answers.at(MSG_USER_IS_LEADER);
                }
            }
        }
        // If user Viewer or not a leader of its covenant
        if (answer.isEmpty())
        {
            // Get amount of currency which user have
            int currency = UD_GET_PARAM(message.GetRealName(), UDP_Currency).toInt();
            // Set default price value
            int priceToJoin(100);
            // Try to get param from config manager
            if (configMng.GetStringParam(CFGP_COV_JOIN_PRICE,price))
            {
                priceToJoin = price.toInt();

            }
            // If user have enough currency to join
            if (currency >= priceToJoin)
            {
                // Get covenant list
                QStringList covenants;
                DB_QUERY_PTR query = DB_SELECT("Covenants", "Name");
                if (query->exec())
                {
                    while (query->next())
                    {
                        covenants.append(query->value("Name").toString());
                    }
                }
                // Try to find specified covenant from list of covenants
                for (auto iter = covenants.begin(); iter != covenants.end(); ++iter)
                {
                    // Found a overlap
                    if (message.GetMessage().contains(*iter))
                    {
                        // Check if user already in this covenant
                        if (UD_GET_PARAM(message.GetRealName(), UDP_Covenant) != *iter)
                        {
                            // Join user to covenant and take currency for it
                            answer = _answers.at(MSG_JOINING_COV);
                            answer.replace("COV_NAME", *iter);
                            UD_UPDATE(message.GetRealName(), UDP_Covenant, *iter);
                            UD_UPDATE(message.GetRealName(), UDP_Currency, QString::number(currency - priceToJoin));
                        }
                        // User is already in that covenant
                        else
                        {
                            // Second answer
                            answer = _answers.at(MSG_ALREADY_IN_COV);
                        }
                        break;
                    }
                }
            }
            else
            {
                answer = _answers.at(MSG_NO_CURRENCY);
            }
        }
    }
    if (!answer.isEmpty())
    {
        _AddAuthorName(answer, message.GetAuthor());
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////