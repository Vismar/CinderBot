/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "JoinCovenantCommand.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

enum
{
    MsgJoiningCov = 0,
    MsgAlreadyInCov,
    MsgNoCurrency,
    MsgUserIsLeader,
    MsgCovenantFull,
    MsgCovenantNotExist,
    MsgCovNotSpecified
};

///////////////////////////////////////////////////////////////////////////

JoinCovenantCommand::JoinCovenantCommand()
{
    _covNameExpression.setPattern("!cov_join (?<covenant>.*)");
    _name = "!cov_join";
    _answers.push_back("You are joining 'COV_NAME', @.");
    _answers.push_back("You are already in that covenant, @.");
    _answers.push_back("Not enough currency, @.");
    _answers.push_back("You are leader of your covenant, @! Please leave before trying to join.");
    _answers.push_back("Covenant is full, @.");
    _answers.push_back("Such covenant do not exist, @.");
    _answers.push_back("You do not specified a covenant to join in, @.");
}

///////////////////////////////////////////////////////////////////////////

void JoinCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    // Try to find the specified covenant
    QRegularExpressionMatch match = _covNameExpression.match(message.GetMessage());
    if (match.hasMatch())
    {
        // Save covenant if it was specified
        QString specifiedCovenant = match.captured("covenant");

        QString price;
        ConfigurationManager &configMng = ConfigurationManager::Instance();
        QString covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();

        // Check if user is leader of its covenant
        if (covenant != "Viewer")
        {
            // If user is leader, notify him about it
            if (CovenantDBHelper::CheckLeadership(message.GetUserID()))
            {
                answer.AddAnswer(_answers.at(MsgUserIsLeader));
            }
        }
        // If user Viewer or not a leader of its covenant
        if (answer.GetAnswers().isEmpty())
        {
            // Set default price value
            int priceToJoin(100);

            // Try to get param from config manager
            if (configMng.GetStringParam(CfgParam::CovJoinPrice, price))
            {
                priceToJoin = price.toInt();

            }
            _price = priceToJoin;

            // If user have enough currency to join
            if (_CheckCurrency(message.GetUserID()))
            {
                // Check if specified covenant exist
                if (CovenantDBHelper::IsCovenantExist(specifiedCovenant))
                {
                    // Check if user already in this covenant
                    if (covenant != specifiedCovenant)
                    {
                        // Maximum of members in selected covenant
                        int maxMembers = CovenantDBHelper::GetParameter(CovenantParameter::MaxMembers, specifiedCovenant).toInt();

                        // If covenant full, notify about it
                        if (maxMembers <= UserDataDBHelper::GetUsersFromCovenant(specifiedCovenant).size())
                        {
                            answer.AddAnswer(_answers.at(MsgCovenantFull));
                        }
                        else
                        {
                            // Join user to covenant and take currency for it
                            answer.AddAnswer(_answers.at(MsgJoiningCov));
                            (*answer.GetAnswers().begin()).replace("COV_NAME", specifiedCovenant);
                            UserDataDBHelper::UpdateUserParameter(UserDataParameter::Covenant, specifiedCovenant, message.GetUserID());
                            _TakeDefaultPriceFromUser(message.GetUserID());
                        }
                    }
                    // User is already in that covenant
                    else
                    {
                        // Second answer
                        answer.AddAnswer(_answers.at(MsgAlreadyInCov));
                    }
                }
            }
            else
            {
                answer.AddAnswer(_answers.at(MsgCovenantNotExist));
            }
        }
        else
        {
            answer.AddAnswer(_answers.at(MsgNoCurrency));
        }
    }
    else
    {
        answer.AddAnswer(_answers.at(MsgCovNotSpecified));
    }
}

///////////////////////////////////////////////////////////////////////////

void JoinCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
