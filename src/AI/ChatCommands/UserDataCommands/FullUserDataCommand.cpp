/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "FullUserDataCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/DatabaseManager.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Command::UserDataCmd;

#define MSG_NOT_LEADER 0
#define MSG_IS_LEADER  1

///////////////////////////////////////////////////////////////////////////

FullUserDataCommand::FullUserDataCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void FullUserDataCommand::Initialize()
{
    _name = "!me";
    _answers.push_back("@: Message - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; Covenant - MSG_COV.");
    _answers.push_back("@: Message - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; Leader of 'MSG_COV' covenant.");
}

///////////////////////////////////////////////////////////////////////////

void FullUserDataCommand::_GetAnswer(const ChatMessage &message, QStringList &answer)
{
    QString covenant = UD_GET_PARAM(message.GetRealName() ,UDP_Covenant);
    DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
    if (query != nullptr)
    {
        if (query->first())
        {
            if (query->value("Leader").toString() == message.GetRealName())
            {
                answer.append(_answers.at(MSG_IS_LEADER));
            }
        }
    }
    if (answer.isEmpty())
    {
        answer.append(_answers.at(MSG_NOT_LEADER));
    }
    if (!answer.isEmpty())
    {
        auto firstAnswer = answer.begin();
        QString curName = "NomNom ";
        ConfigurationManager::Instance().GetStringParam(CFGP_CURRENCY, curName);
        (*firstAnswer).replace("MSG_NAME_CUR", curName);
        (*firstAnswer).replace("MSG_COUNT", UD_GET_PARAM(message.GetRealName() ,UDP_Messages));
        (*firstAnswer).replace("MSG_CUR", UD_GET_PARAM(message.GetRealName() ,UDP_Currency));
        (*firstAnswer).replace("MSG_COV", UD_GET_PARAM(message.GetRealName() ,UDP_Covenant));
    }
}

///////////////////////////////////////////////////////////////////////////

void FullUserDataCommand::_GetRandomAnswer(const ChatMessage &message, QStringList &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
