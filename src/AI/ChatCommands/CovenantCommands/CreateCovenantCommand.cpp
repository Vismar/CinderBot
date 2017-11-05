/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CreateCovenantCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

enum
{
    MsgNoCurrency = 0,
    MsgUserIsLeader,
    MsgCovenantCreated,
    MsgProvideName,
    MsgAlreadyExist
};

#define MAX_COVENANT_NAME_LENGTH 50
#define DEFAULT_PRICE_FOR_CREATE "2000"

///////////////////////////////////////////////////////////////////////////

CreateCovenantCommand::CreateCovenantCommand()
{
    _name = "!cov_create";
    _answers.push_back("You do not have enought currency, @.");
    _answers.push_back("You are leader of another covenant, @! Leave or disband it and try again.");
    _answers.push_back("Covenant 'COV_NAME' was created. @! Now you are leader of it.");
    _answers.push_back("Please provide name of new covenant, @.");
    _answers.push_back("Such covenant already exist, @.");
}

///////////////////////////////////////////////////////////////////////////

void CreateCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    QString covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();
    // Check if user is leader of its covenant
    if (CovenantDBHelper::CheckLeadership(message.GetUserID()))
    {
        // User is leader of another covenant
        answer.AddAnswer(_answers.at(MsgUserIsLeader));
    }
    // If user is not a leader or he not in a covenant, he is allowed to create covenant
    else
    {

        // Get amount currency which is needed to create covenant
        QString price;
        if (!ConfigurationManager::Instance().GetStringParam(CfgParam::CovCreatePrice, price))
        {
            price = DEFAULT_PRICE_FOR_CREATE;
        }
        _price = price.toInt();

        // Check if user have enough currency
        if (_CheckCurrency(message.GetUserID()))
        {
            // Get covenant name
            QString newCovenant;
            if (message.GetMessage().size() > _name.size())
            {
                newCovenant = message.GetMessage().right(message.GetMessage().size() - _name.size() - 1);
                newCovenant.replace("'", "");
            }

            // If covenant name too long, just make it shorter
            if (newCovenant.size() > MAX_COVENANT_NAME_LENGTH)
            {
                newCovenant.left(MAX_COVENANT_NAME_LENGTH);
            }

            // If user provided covenant name, create it
            if (!newCovenant.isEmpty())
            {
                // Check if such covenant already exist
                if (CovenantDBHelper::IsCovenantExist(newCovenant))
                {
                    answer.AddAnswer(_answers.at(MsgAlreadyExist));
                }
                // Create new covenant if such covenant not exist
                else
                {
                    if (CovenantDBHelper::CreateCovenant(newCovenant, message.GetRealName()))
                    {
                        // Update covenant field for user
                        UserDataDBHelper::UpdateUserParameter(UserDataParameter::Covenant, newCovenant, message.GetUserID());
                        // Take price to create covenant
                        _TakeDefaultPriceFromUser(message.GetUserID());
                        // Set answer
                        answer.AddAnswer(_answers.at(MsgCovenantCreated));
                        (*answer.GetAnswers().begin()).replace("COV_NAME", newCovenant);
                    }
                }
            }
            // Name of covenant doesn't provided
            else
            {
                answer.AddAnswer(_answers.at(MsgProvideName));
            }
        }
        // Not enough currency
        else
        {
            answer.AddAnswer(_answers.at(MsgNoCurrency));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CreateCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
