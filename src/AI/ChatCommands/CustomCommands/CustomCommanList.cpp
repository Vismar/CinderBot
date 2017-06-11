/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCommandList.hpp"
#include "AI/ChatCommands/CustomCommands/CustomChatCommand.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CustomCommandList::CustomCommandList()
{
    _commandTableName = "CustomCommands";
    _commandAnswersTableName = "CustomCommandAnswers";
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

CustomCommandList::~CustomCommandList() { }

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_Initialize()
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

    _InitializeCommands();
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_InitializeCommands()
{
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
