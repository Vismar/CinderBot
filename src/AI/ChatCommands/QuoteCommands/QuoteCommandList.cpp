/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AI/ChatCommands/QuoteCommands/QuoteCommandList.hpp"
#include "AI/ChatCommands/QuoteCommands/MainQuoteCommand.hpp"
#include "AI/ChatCommands/QuoteCommands/AddQuoteCommand.hpp"
#include "AI/ChatCommands/QuoteCommands/DeleteQuoteCommand.hpp"
#include "AI/ChatCommands/QuoteCommands/EditQuoteCommand.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

QuoteCommandList::QuoteCommandList()
{
    _commands.push_back(new MainQuoteCommand());
    _commands.push_back(new AddQuoteCommand());
    _commands.push_back(new DeleteQuoteCommand());
    _commands.push_back(new EditQuoteCommand());

    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

QuoteCommandList::~QuoteCommandList() {}

///////////////////////////////////////////////////////////////////////////

void QuoteCommandList::_Initialize()
{
    // Initialize data table in database
    DB_CREATE_TABLE("Quotes", "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "Quote TEXT NOT NULL,"
                              "Number INTEGER NOT NULL UNIQUE");

    std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "id", "number = 0");
    if (!numberQuery->next())
    {
        DB_INSERT("Quotes", "NULL, 'Technical quote with 0 number for purpose of max number', 0", true);
    }
}

///////////////////////////////////////////////////////////////////////////
