/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CreateCovenantCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

#define MSG_NO_CURRENCY      0
#define MSG_USER_IS_LEADER   1
#define MSG_COVENANT_CREATED 2
#define MSG_PROVIDE_NAME     3
#define MSG_ALREADY_EXIST    4

#define MAX_COVENANT_NAME_LENGTH 50
#define DEFAULT_PRICE_FOR_CREATE "2000"

///////////////////////////////////////////////////////////////////////////

CreateCovenantCommand::CreateCovenantCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CreateCovenantCommand::Initialize()
{
    _name = "!cov_create";
    _answers.push_back("You do not have enought currency, @.");
    _answers.push_back("You are leader of another covenant, @! Leave or disband it and try again.");
    _answers.push_back("Covenant 'COV_NAME' was created. @! Now you are leader of it.");
    _answers.push_back("Please provide name of new covenant, @.");
    _answers.push_back("Such covenant already exist, @.");
}

///////////////////////////////////////////////////////////////////////////

void CreateCovenantCommand::_GetAnswer(const ChatMessage& message, QStringList& answer)
{
    QString covenant = UD_GET_PARAM(message.GetRealName(), UDP_Covenant);
    // Check if user is leader of its covenant
    DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
    if (query->exec())
    {
        // If user is in covenant, check its leader
        if (query->first())
        {
            if (query->value("Leader").toString() == message.GetRealName())
            {
                // User is leader of another covenant
                answer.append(_answers.at(MSG_USER_IS_LEADER));
            }
        }
        // If user is not a leader or he not in a covenant, he is allowed to create covenant
        if (answer.isEmpty())
        {
            // Get amount currency which is needed to create covenant
            QString price;
            if (!ConfigurationManager::Instance().GetStringParam(CFGP_COV_CREATE_PRICE, price))
            {
                price = DEFAULT_PRICE_FOR_CREATE;
            }
            _price = price.toInt();
            // Check if user have enough currency
            if (_CheckCurrency(message.GetRealName()))
            {
                // Get covenant name
                QString newCovenant = message.GetMessage().mid(_name.size()+1);
                int covNameLength = ((newCovenant.size()-2) < 0) ? 0 : (newCovenant.size()-2);
                // If covenant name too long, just make it shorter
                if (covNameLength > MAX_COVENANT_NAME_LENGTH)
                {
                    covNameLength = MAX_COVENANT_NAME_LENGTH;
                }
                newCovenant = newCovenant.left(covNameLength);
                // If user provided covenant name, create it
                if (!newCovenant.isEmpty())
                {
                    // Check if such covenant already exist
                    DB_QUERY_PTR queryName = DB_SELECT("Covenants", "COUNT(*)",
                                                                    QString("Name = '%1'").arg(newCovenant));
                    if (queryName != NULL)
                    {
                        if (queryName->first())
                        {
                            if (queryName->value(0).toInt() != 0)
                            {
                                answer.append(_answers.at(MSG_ALREADY_EXIST));
                            }
                        }
                    }

                    // Create new covenant if such covenant not exist
                    if ((answer.isEmpty()) &&
                        (DB_INSERT("Covenants", QString("NULL, '%1', '%2', "
                                                        "'', 1, 1, 0").arg(newCovenant).arg(message.GetRealName()))))
                    {
                        // Update covenant field for user
                        UD_UPDATE(message.GetRealName(), UDP_Covenant, newCovenant);
                        // Take price to create covenant
                        _TakeDefaultPriceFromUser(message.GetRealName());
                        // Set answer
                        answer.append(_answers.at(MSG_COVENANT_CREATED));
                        (*answer.begin()).replace("COV_NAME", newCovenant);
                    }
                }
                // Name of covenant doesn't provided
                else
                {
                    answer.append(_answers.at(MSG_PROVIDE_NAME));
                }
            }
            // Not enough currency
            else
            {
                answer.append(_answers.at(MSG_NO_CURRENCY));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CreateCovenantCommand::_GetRandomAnswer(const ChatMessage& message, QStringList& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
