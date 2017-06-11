/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UserDataCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

void UserDataCommand::SetCommandType(UDCommandType cmdType)
{
    _cmdType = cmdType;
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void UserDataCommand::_GetAnswer(const ChatMessage& message, QStringList& answer)
{
    ConfigurationManager& configMng = ConfigurationManager::Instance();
    answer.append(_answers.first());
    auto firstAnswer = answer.begin();
    (*firstAnswer).replace("MSG_COUNT", UD_GET_PARAM(message.GetRealName() ,UDP_Messages));
    (*firstAnswer).replace("MSG_CUR", UD_GET_PARAM(message.GetRealName() ,UDP_Currency));
    QString curName = "NomNom ";
    configMng.GetStringParam(CFGP_CURRENCY, curName);
    (*firstAnswer).replace("MSG_NAME_CUR", curName);
    (*firstAnswer).replace("MSG_COV", UD_GET_PARAM(message.GetRealName() ,UDP_Covenant));
}

///////////////////////////////////////////////////////////////////////////

void UserDataCommand::_GetRandomAnswer(const ChatMessage& message, QStringList& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////

void UserDataCommand::Initialize()
{
    _Clear();
    switch (_cmdType)
    {
    case UDC_Messages:
        _name = "!ud_msg";
        _answers.push_back("You wrote MSG_COUNT messages! @");
        break;
    case UDC_Currency:
        _name = "!ud_cur";
        _answers.push_back("You have MSG_CUR of MSG_NAME_CUR ! @");
        break;
    case UDC_Covenant:
        _name = "!ud_cov";
        _answers.push_back("You are in MSG_COV covenant! @");
        break;
    case UDC_Full:
        _name = "!me";
        _answers.push_back("@: Message - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; Covenant - MSG_COV.");
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
