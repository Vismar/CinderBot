/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCovCommandList.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "AI/ChatCommands/CustomCommands/CustomCovChatCommand.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Command::CustomChatCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CustomCovCommandList::CustomCovCommandList()
{
    _commandTableName = "CustomCovCommands";
    _commandAnswersTableName = "CustomCovCommandAnswers";
    OnCfgParamChanged(CfgParam::CovenantCmdModule);
}

///////////////////////////////////////////////////////////////////////////

void CustomCovCommandList::OnCfgParamChanged(CfgParam cfgParam)
{
    QString value;
    switch (cfgParam)
    {
    case CfgParam::CovenantCmdModule:
        ConfigurationManager::Instance().GetStringParam(CfgParam::CovenantCmdModule, value);
        _isTurnedOn = ("true" == value);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCovCommandList::_InitializeCommands()
{
    // Clear all commands that was already created
    for (int i = 0; i < _commands.size(); ++i)
    {
        delete _commands[i];
    }
    _commands.clear();

    DB_QUERY_PTR query = DB_SELECT(_commandTableName, "Name");
    if (query != NULL)
    {
        while (query->next())
        {
            CustomChatCommand* chatCommand = new CustomCovChatCommand();
            chatCommand->InitializeByName(query->value("Name").toString());
            connect (&CustomCommandDBHelper::Instance(), &CustomCommandDBHelper::CustomCovCmdParameterChanged,
                     chatCommand, &CustomChatCommand::OnParameterChanged);
            _commands.push_back(chatCommand);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
