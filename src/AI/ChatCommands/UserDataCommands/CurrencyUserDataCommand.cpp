/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CurrencyUserDataCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CurrencyUserDataCommand::CurrencyUserDataCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CurrencyUserDataCommand::Initialize()
{
    _name = "!cur";
    _answers.push_back("You have MSG_CUR of MSG_NAME_CUR , @!");
}

///////////////////////////////////////////////////////////////////////////

void CurrencyUserDataCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    answer.AddAnswer(_answers.first());
    QString currency = QString::number(UserDataDBHelper::GetUserParameter(UserDataParameter::Currency, message.GetUserID()).toInt());
    (*answer.GetAnswers().begin()).replace("MSG_CUR", currency);
    QString curName = "NomNom ";
    ConfigurationManager::Instance().GetStringParam(CfgParam::Currency, curName);
    (*answer.GetAnswers().begin()).replace("MSG_NAME_CUR", curName);
}

///////////////////////////////////////////////////////////////////////////

void CurrencyUserDataCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
