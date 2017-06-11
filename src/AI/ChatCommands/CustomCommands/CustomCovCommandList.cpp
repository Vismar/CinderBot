/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCovCommandList.hpp"
#include "AI/ChatCommands/CustomCommands/CustomCovChatCommand.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CustomCovCommandList::CustomCovCommandList()
{
    _commandTableName = "CustomCovCommands";
    _commandAnswersTableName = "CustomCovCommandAnswers";
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CustomCovCommandList::_InitializeCommands()
{
    DB_QUERY_PTR query = DB_SELECT(_commandTableName, "Name");
    if (query != NULL)
    {
        while (query->next())
        {
            CustomChatCommand* chatCommand = new CustomCovChatCommand();
            chatCommand->InitializeByName(query->value("Name").toString());
            _commands.push_back(chatCommand);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
