/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UserDataBaseCommand.hpp"
#include "Utils/Config/ConfigurationManager.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Configuration;

///////////////////////////////////////////////////////////////////////////

UserDataBaseCommand::UserDataBaseCommand()
{
    QString value;
    ConfigurationManager::Instance().GetStringParam(CfgParam::UserDataCmdRcvChat, value);
    _workInChat = ("true" == value);
    value.clear();
    ConfigurationManager::Instance().GetStringParam(CfgParam::UserDataCmdRcvWhisper, value);
    _workInWhisper = ("true" == value);

    connect(&ConfigurationManager::Instance(), &ConfigurationManager::ParameterChanged,
            this, &UserDataBaseCommand::_UpdateChatAndWhisperFlags);
}

///////////////////////////////////////////////////////////////////////////

UserDataBaseCommand::~UserDataBaseCommand() { }

///////////////////////////////////////////////////////////////////////////

void UserDataBaseCommand::_UpdateChatAndWhisperFlags(CfgParam cfgParam)
{
    QString value;
    switch (cfgParam)
    {
    case CfgParam::UserDataCmdRcvChat:
        ConfigurationManager::Instance().GetStringParam(CfgParam::UserDataCmdRcvChat, value);
        _workInChat = ("true" == value);
        break;
    case CfgParam::UserDataCmdRcvWhisper:
        ConfigurationManager::Instance().GetStringParam(CfgParam::UserDataCmdRcvWhisper, value);
        _workInWhisper = ("true" == value);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
