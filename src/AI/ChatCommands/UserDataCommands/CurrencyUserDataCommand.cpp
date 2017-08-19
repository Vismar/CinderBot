/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CurrencyUserDataCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Configuration;

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
    (*answer.GetAnswers().begin()).replace("MSG_CUR", UD_GET_PARAM(message.GetRealName(), UDP_Currency));
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
