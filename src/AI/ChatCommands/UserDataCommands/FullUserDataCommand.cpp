/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "FullUserDataCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

#define MSG_NOT_LEADER 0
#define MSG_NO_COV     1
#define MSG_IS_LEADER  2

///////////////////////////////////////////////////////////////////////////

FullUserDataCommand::FullUserDataCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void FullUserDataCommand::Initialize()
{
    _name = "!me";
    _answers.push_back("@: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Covenant - MSG_COV.");
    _answers.push_back("@: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Not in covenant.");
    _answers.push_back("@: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Leader of covenant 'MSG_COV'.");
}

///////////////////////////////////////////////////////////////////////////

void FullUserDataCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    UserDataParams userDataParams = UserDataDBHelper::GetUserParameters(message.GetUserID());
    DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(userDataParams.Covenant));
    if (query != nullptr)
    {
        if (query->first())
        {
            if (query->value("Leader").toString() == message.GetRealName())
            {
                answer.AddAnswer(_answers.at(MSG_IS_LEADER));
            }
        }
    }
    if (answer.GetAnswers().isEmpty())
    {
        if (userDataParams.Covenant == "Viewer")
        {
            answer.AddAnswer(_answers.at(MSG_NO_COV));
        }
        else
        {
            answer.AddAnswer(_answers.at(MSG_NOT_LEADER));
        }
    }
    if (!answer.GetAnswers().isEmpty())
    {
        auto firstAnswer = answer.GetAnswers().begin();
        QString curName = "NomNom ";
        ConfigurationManager::Instance().GetStringParam(CfgParam::Currency, curName);
        (*firstAnswer).replace("MSG_NAME_CUR", curName);
        (*firstAnswer).replace("MSG_COUNT", QString::number(userDataParams.Messages));
        (*firstAnswer).replace("MSG_CUR", QString::number(userDataParams.Currency));
        (*firstAnswer).replace("MSG_COV", userDataParams.Covenant);
        // Time in chat
        int minutes = userDataParams.MinutesInChat;
        int hours = minutes / 60;
        minutes %= 60;
        (*answer.GetAnswers().begin()).replace("MSG_TIME_IN_CHAT", QString("%1h%2m").arg(hours).arg(minutes));
    }
}

///////////////////////////////////////////////////////////////////////////

void FullUserDataCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
