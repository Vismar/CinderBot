/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UserDataCommandList.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "AI/ChatCommands/UserDataCommands/MessagesUserDataCommand.hpp"
#include "AI/ChatCommands/UserDataCommands/CurrencyUserDataCommand.hpp"
#include "AI/ChatCommands/UserDataCommands/CovenantUserDataCommand.hpp"
#include "AI/ChatCommands/UserDataCommands/TimeInChatCommand.hpp"
#include "AI/ChatCommands/UserDataCommands/FullUserDataCommand.hpp"
#include "AI/ChatCommands/UserDataCommands/CheckUserDataCommand.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Configuration;

///////////////////////////////////////////////////////////////////////////

UserDataCommandList::UserDataCommandList()
{
    Initialize();
    OnCfgParamChanged(UserDataCmdModule);
}

///////////////////////////////////////////////////////////////////////////

void UserDataCommandList::Initialize()
{
    _commands.push_back(new MessagesUserDataCommand());
    _commands.push_back(new CurrencyUserDataCommand());
    _commands.push_back(new CovenantUserDataCommand());
    _commands.push_back(new TimeInChatCommand());
    _commands.push_back(new FullUserDataCommand());
    _commands.push_back(new CheckUserDataCommand());
}

///////////////////////////////////////////////////////////////////////////

void UserDataCommandList::OnCfgParamChanged(CfgParam cfgParam)
{
    QString value;
    switch (cfgParam)
    {
    case UserDataCmdModule:
        ConfigurationManager::Instance().GetStringParam(UserDataCmdModule, value);
        _isTurnedOn = ("true" == value);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
