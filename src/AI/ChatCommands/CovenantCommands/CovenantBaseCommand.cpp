/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CovenantBaseCommand.hpp"
#include "Utils/Config/ConfigurationManager.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Configuration;

///////////////////////////////////////////////////////////////////////////

CovenantBaseCommand::CovenantBaseCommand()
{
    QString value;
    ConfigurationManager::Instance().GetStringParam(CfgParam::CovenantCmdRcvChat, value);
    _workInChat = ("true" == value);
    value.clear();
    ConfigurationManager::Instance().GetStringParam(CfgParam::CovenantCmdRcvWhisper, value);
    _workInWhisper = ("true" == value);

    connect(&ConfigurationManager::Instance(), &ConfigurationManager::ParameterChanged,
            this, &CovenantBaseCommand::_UpdateChatAndWhisperFlags);
}

///////////////////////////////////////////////////////////////////////////

CovenantBaseCommand::~CovenantBaseCommand() { }

///////////////////////////////////////////////////////////////////////////

void CovenantBaseCommand::_UpdateChatAndWhisperFlags(CfgParam cfgParam)
{
    QString value;
    switch (cfgParam)
    {
    case CfgParam::CovenantCmdRcvChat:
        ConfigurationManager::Instance().GetStringParam(CfgParam::CovenantCmdRcvChat, value);
        _workInChat = ("true" == value);
        break;
    case CfgParam::CovenantCmdRcvWhisper:
        ConfigurationManager::Instance().GetStringParam(CfgParam::CovenantCmdRcvWhisper, value);
        _workInWhisper = ("true" == value);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
