/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCommandList.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "AI/ChatCommands/CustomCommands/CustomChatCommand.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command::CustomChatCmd;
using namespace Utils::Configuration;

///////////////////////////////////////////////////////////////////////////

CustomCommandList::CustomCommandList()
{
    _commandTableName = "CustomCommands";
    _commandAnswersTableName = "CustomCommandAnswers";
    OnCfgParamChanged(CfgParam::CustomCmdModule);
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_UpdateCommands(const QString &tableName)
{
    if (tableName == _commandTableName)
    {
        _InitializeCommands();
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::Initialize()
{
    DB_CREATE_TABLE(_commandTableName, "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "Name TEXT NOT NULL,"
                                       "Cooldown TEXT NOT NULL,"
                                       "ModeratorOnly INTEGER NOT NULL,"
                                       "Price INTEGER NOT NULL,"
                                       "Covenant TEXT NOT NULL");

    DB_CREATE_TABLE(_commandAnswersTableName, "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                              "Name TEXT NOT NULL,"
                                              "Answer TEXT NOT NULL");

    DB_CREATE_INDEX(_commandAnswersTableName, "Answer_Index", "Answer");

    // Connect events from database manager to know, when we need to update commands
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnInsertEvent,
            this, &CustomCommandList::_UpdateCommands);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnUpdateEvent,
            this, &CustomCommandList::_UpdateCommands);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnDeleteEvent,
            this, &CustomCommandList::_UpdateCommands);

    _InitializeCommands();
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::OnCfgParamChanged(CfgParam cfgParam)
{
    QString value;
    switch (cfgParam)
    {
    case CfgParam::CustomCmdModule:
        ConfigurationManager::Instance().GetStringParam(CfgParam::CustomCmdModule, value);
        _isTurnedOn = ("true" == value);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_InitializeCommands()
{
    // Clear all commands that was already created
    for (int i = 0; i < _commands.size(); ++i)
    {
        delete _commands[i];
    }
    _commands.clear();

    // Create new commands
    DB_QUERY_PTR query = DB_SELECT(_commandTableName, "Name");
    if (query != NULL)
    {
        while (query->next())
        {
            CustomChatCommand* chatCommand = new CustomChatCommand();
            chatCommand->InitializeByName(query->value("Name").toString());
            _commands.push_back(chatCommand);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
