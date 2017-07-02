/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CovenantUserDataCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command::UserDataCmd;

#define MSG_COVENANT  0
#define MSG_IS_LEADER 1

///////////////////////////////////////////////////////////////////////////

CovenantUserDataCommand::CovenantUserDataCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CovenantUserDataCommand::Initialize()
{
    _name = "!cov";
    _answers.push_back("You are in covenant 'MSG_COV', @!");
    _answers.push_back(" And you are leader of it!");
}

///////////////////////////////////////////////////////////////////////////

void CovenantUserDataCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    answer.AddAnswer(_answers.at(MSG_COVENANT));
    QString covenant = UD_GET_PARAM(message.GetRealName() ,UDP_Covenant);
    (*answer.GetAnswers().begin()).replace("MSG_COV", covenant);
    DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
    if (query != nullptr)
    {
        if (query->first())
            {
            if (query->value("Leader").toString() == message.GetRealName())
            {
                (*answer.GetAnswers().begin()).append(_answers.at(MSG_IS_LEADER));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CovenantUserDataCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
