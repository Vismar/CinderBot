#include "QuoteCommandList.hpp"
#include "MainQuoteCommand.hpp"
#include "AddQuoteCommand.hpp"
#include "DeleteQuoteCommand.hpp"
#include "EditQuoteCommand.hpp"
#include <Utils/DatabaseManager.hpp>
#include <QFile>

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
    DB_CREATE_TABLE("Quotes", "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "quote TEXT NOT NULL,"
                              "number INTEGER NOT NULL UNIQUE");

    std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "id", "number = 0");
    if (!numberQuery->next())
    {
        DB_INSERT("Quotes", "NULL, 'Technical quote with 0 number for purpose of max number', 0", true);
    }
}

///////////////////////////////////////////////////////////////////////////
