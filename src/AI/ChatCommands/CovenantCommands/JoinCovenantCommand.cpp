/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "JoinCovenantCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include "Utils/DatabaseManager.hpp"

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

void JoinCovenantCommand::_GetAnswer(const ChatMessage& message, QStringList& answer)
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
                answer.append(_answers.at(MSG_USER_IS_LEADER));
            }
        }
    }
    // If user Viewer or not a leader of its covenant
    if (answer.isEmpty())
    {
        // Set default price value
        int priceToJoin(100);
        // Try to get param from config manager
        if (configMng.GetStringParam(CFGP_COV_JOIN_PRICE,price))
        {
            priceToJoin = price.toInt();

        }
        _price = priceToJoin;
        // If user have enough currency to join
        if (_CheckCurrency(message.GetRealName()))
        {
            // Get covenant list
            DB_QUERY_PTR query = DB_SELECT("Covenants", "Name");
            if (query != nullptr)
            {
                QStringList covenants;
                while (query->next())
                {
                    covenants.append(query->value("Name").toString());
                }
                // Try to find specified covenant from list of covenants
                for (auto iter = covenants.begin(); iter != covenants.end(); ++iter)
                {
                    // Found an overlap
                    if (message.GetMessage().contains(*iter))
                    {
                        // Check if user already in this covenant
                        if (UD_GET_PARAM(message.GetRealName(), UDP_Covenant) != *iter)
                        {
                            // Join user to covenant and take currency for it
                            answer.append(_answers.at(MSG_JOINING_COV));
                            (*answer.begin()).replace("COV_NAME", *iter);
                            UD_UPDATE(message.GetRealName(), UDP_Covenant, *iter);
                            _TakeDefaultPriceFromUser(message.GetRealName());
                        }
                        // User is already in that covenant
                        else
                        {
                            // Second answer
                            answer.append(_answers.at(MSG_ALREADY_IN_COV));
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            answer.append(_answers.at(MSG_NO_CURRENCY));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void JoinCovenantCommand::_GetRandomAnswer(const ChatMessage& message, QStringList& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
