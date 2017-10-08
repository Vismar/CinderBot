/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CheckUserDataCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

enum
{
    MsgNotLeader = 0,
    MsgNoCov,
    MsgIsLeader,
    MsgNoName,
    MsgNoUser
};

///////////////////////////////////////////////////////////////////////////

CheckUserDataCommand::CheckUserDataCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CheckUserDataCommand::Initialize()
{
    _name = "!check";
    _moderatorOnly = true;
    _answers.push_back("USER_NAME: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Covenant - MSG_COV.");
    _answers.push_back("USER_NAME: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Not in covenant.");
    _answers.push_back("USER_NAME: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Leader of covenant 'MSG_COV'.");
    _answers.push_back("Please provide a name, @!");
    _answers.push_back("User with such name is not exist, @!");
}

///////////////////////////////////////////////////////////////////////////

void CheckUserDataCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    if (_CheckModerationFlag(message.IsModerator()))
    {
        QString userName;
        if (message.GetMessage().size() > _name.size())
        {
            userName = message.GetMessage().right(message.GetMessage().size() - _name.size() - 1);
        }
        if (!userName.isEmpty())
        {
            UserDataParams userDataParams = UserDataDBHelper::GetUserParameters(userName);
            // If user exist, get data
            if (userDataParams.UserID != -1)
            {
                // Check if user is covenant leader
                DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name='%1'").arg(userDataParams.Covenant));
                if (query != nullptr)
                {
                    if (query->first())
                    {
                        if (query->value("Leader").toString() == userDataParams.Name)
                        {
                            answer.AddAnswer(_answers.at(MsgIsLeader));
                        }
                    }
                }

                // If user is not leader of covenant, than check user covenant
                if (answer.GetAnswers().isEmpty())
                {
                    if (userDataParams.Covenant == "Viewer")
                    {
                        answer.AddAnswer(_answers.at(MsgNoCov));
                    }
                    else
                    {
                        answer.AddAnswer(_answers.at(MsgNotLeader));
                    }
                }

                // If command was executed, replace placeholders with actual info
                if (!answer.GetAnswers().isEmpty())
                {
                    auto firstAnswer = answer.GetAnswers().begin();
                    
                    // Get currency type
                    QString curName = "NomNom ";
                    ConfigurationManager::Instance().GetStringParam(CfgParam::Currency, curName);

                    (*firstAnswer).replace("MSG_NAME_CUR", curName);
                    (*firstAnswer).replace("MSG_COUNT", QString::number(userDataParams.Messages));
                    (*firstAnswer).replace("MSG_CUR", QString::number(userDataParams.Currency));
                    (*firstAnswer).replace("MSG_COV", userDataParams.Covenant);
                    (*firstAnswer).replace("USER_NAME", userName);

                    // Time in chat
                    int minutes = userDataParams.MinutesInChat;
                    int hours = minutes / 60;
                    minutes %= 60;
                    (*answer.GetAnswers().begin()).replace("MSG_TIME_IN_CHAT", QString("%1h%2m").arg(hours).arg(minutes));
                }
            }
            // If provided users is not exist in DB, notify about it
            else
            {
                answer.AddAnswer(_answers.at(MsgNoUser));
            }
        }
        else
        {
            answer.AddAnswer(_answers.at(MsgNoName));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CheckUserDataCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
