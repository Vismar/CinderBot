/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCommandList.hpp"
#include "Utils/Config/ConfigurationManager.hpp"

using namespace Command::CustomChatCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CustomCommandList::CustomCommandList()
: _commandType(CmdType::StreamerCmd)
, _cmdModule(CfgParam::CustomCmdModule)
{
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::Initialize()
{
    // Connect events from CustomCommandDBHelper yo know, when we need to add or remove commands
    connect(&CustomCommandDBHelper::Instance(), &CustomCommandDBHelper::CustomCmdAdded,
            this, &CustomCommandList::_AddCommand);
    connect(&CustomCommandDBHelper::Instance(), &CustomCommandDBHelper::CustomCmdDeleted,
        this, &CustomCommandList::_DeleteCommand);

    _InitializeCommands();
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::OnCfgParamChanged(CfgParam cfgParam)
{
    QString value;
    if (cfgParam == _cmdModule)
    {
        ConfigurationManager::Instance().GetStringParam(_cmdModule, value);
        _isTurnedOn = ("true" == value);
    }
}

///////////////////////////////////////////////////////////////////////////

CustomChatCommand* CustomCommandList::_CreateCommand() const
{
    return new CustomChatCommand();
}


///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_InitializeCommands()
{
    // Update value before intializing
    OnCfgParamChanged(_cmdModule);
    // Clear all commands that was already created
    for (int i = 0; i < _commands.size(); ++i)
    {
        delete _commands[i];
    }
    _commands.clear();

    // Get all command names and initialize list
    QStringList cmdNames = CustomCommandDBHelper::Instance().GetCommandNames(_commandType);
    for (int i = 0; i < cmdNames.size(); ++i)
    {
        _AddCommandToList(cmdNames[i]);
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_AddCommand(CmdType cmdType, const QString& cmdName, int id)
{
    Q_UNUSED(id);
    if (cmdType == _commandType)
    {
        _AddCommandToList(cmdName);
    }
}


///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_DeleteCommand(CmdType cmdType, const QString& cmdName, int id)
{
    Q_UNUSED(id);
    if (cmdType == _commandType)
    {
        for (int i = 0; i < _commands.size(); ++i)
        {
            if (_commands[i]->GetCommandName() == cmdName)
            {
                delete _commands[i];
                _commands.removeAt(i);

                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_AddCommandToList(const QString& cmdName)
{
    // Create command
    CustomChatCommand* chatCommand = _CreateCommand();
    // Initialize it
    chatCommand->InitializeByName(cmdName);
    // Connect events
    connect(&CustomCommandDBHelper::Instance(), &CustomCommandDBHelper::CustomCovCmdParameterChanged,
        chatCommand, &CustomChatCommand::OnParameterChanged);
    // Add command to command list
    _commands.push_back(chatCommand);
}

///////////////////////////////////////////////////////////////////////////
