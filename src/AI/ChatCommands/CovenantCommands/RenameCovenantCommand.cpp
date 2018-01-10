/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "RenameCovenantCommand.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

enum
{
    MsgNotLeader = 0,
    MsgRenamed,
    MsgNoCovenant,
    MsgNoName,
    MsgNoCurrency
};

#define MAX_COVENANT_NAME_LENGTH 50
#define DEFAULT_PRICE_FOR_RENAME "500"

///////////////////////////////////////////////////////////////////////////

RenameCovenantCommand::RenameCovenantCommand()
{
    _name = "!cov_rename";
    _answers.push_back("You are not leader of your covenant, @!");
    _answers.push_back("Your covenant has been renamed, @.");
    _answers.push_back("You are not in any covenant yet, @.");
    _answers.push_back("Please provide a new name of covenant, @.");
    _answers.push_back("You do not have enough currency, @.");
}

///////////////////////////////////////////////////////////////////////////

void RenameCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    QString covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();
    // Check if user in covenant
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        if (CovenantDBHelper::CheckLeadership(message.GetUserID()))
        {
            // Get amount currency which is needed to rename covenant
            QString price;
            if (!ConfigurationManager::Instance().GetStringParam(CfgParam::CovRenamePrice, price))
            {
                price = DEFAULT_PRICE_FOR_RENAME;
            }
            _price = price.toInt();
            if (_CheckCurrency(message.GetUserID()))
            {
                QString newCovenantName = message.GetMessage().mid(_name.size() + 1);
                newCovenantName.replace("'", "");
                // If covenant name too long, just make it shorter
                if (newCovenantName.size() > MAX_COVENANT_NAME_LENGTH)
                {
                    newCovenantName.left(MAX_COVENANT_NAME_LENGTH);
                }
                // If user provided covenant name, rename it
                if (!newCovenantName.isEmpty())
                {
                    // Try to rename covenant
                    if (CovenantDBHelper::RenameCovenant(covenant, newCovenantName))
                    {
                        // If covenant was renamed, set covenant field to a new value for all users who are in that covenant
                        UserDataDBHelper::UpdateCovenantName(covenant, newCovenantName);

                        // Update existing commands for covenant
                        CustomCommandDBHelper::UpdateCovenantName(CmdType::CovenantCmd, covenant, newCovenantName);

                        // Update commands created by broadcaster for covenant
                        CustomCommandDBHelper::UpdateCovenantName(CmdType::StreamerCmd, covenant, newCovenantName);

                        // Take price to rename covenant
                        _TakeDefaultPriceFromUser(message.GetUserID());
                        answer.AddAnswer(_answers.at(MsgRenamed));
                    }
                }
                // If name was not provided
                else
                {
                    answer.AddAnswer(_answers.at(MsgNoName));
                }
            }
            // If user do not have enough currency
            else
            {
                answer.AddAnswer(_answers.at(MsgNoCurrency));
            }
        }
        // If user is not leader
        else
        {
            answer.AddAnswer(_answers.at(MsgNotLeader));
        }
    }
    // If user is not in any covenant
    else
    {
        answer.AddAnswer(_answers.at(MsgNoCovenant));
    }
}

///////////////////////////////////////////////////////////////////////////

void RenameCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
