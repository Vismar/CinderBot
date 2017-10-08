/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "QuoteDBHelper.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Logger.hpp"

using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

QuoteDBHelper& QuoteDBHelper::Instance()
{
    static QuoteDBHelper instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString QuoteDBHelper::InitializeTables()
{
    QString error = "OK";
    // Initialize data table in database
    if (!DB_CREATE_TABLE("Quotes", "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                         "Quote TEXT NOT NULL,"
                         "Number INTEGER NOT NULL UNIQUE"))
    {
        error = QString("Table 'Quotes' was not created.");
    }

    return error;
}

///////////////////////////////////////////////////////////////////////////

int QuoteDBHelper::GetMaxQuoteNumber()
{
    int quoteNumber(0);

    std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(Number)");
    if (numberQuery != nullptr)
    {
        if (numberQuery->first())
        {
            quoteNumber = numberQuery->value(0).toInt();
        }
    }

    return quoteNumber;
}

///////////////////////////////////////////////////////////////////////////

int QuoteDBHelper::AddQuote(const QString& quote)
{
    // Get new quote number before inserting quote itself
    int newQuoteNumber = GetMaxQuoteNumber() + 1;

    // Try to insert quote
    if (DB_INSERT("Quotes", QString("NULL, '%1', %2").arg(quote).arg(newQuoteNumber)))
    {
        // If quote was added, we need to get it od to notify listeners
        DB_QUERY_PTR query = DB_SELECT("Quotes", "Id", QString("Number=%1").arg(newQuoteNumber));
        if ((query != nullptr) && (query->first()))
        {
            // Emit event
            emit QuoteAdded(query->value("Id").toInt());
        }
    }
    // Return -1 as error code and log error itself
    else
    {
        newQuoteNumber = -1;

        // Log error
        LOG(LogError, this->metaObject()->className(), __func__, QString("Cannot add quote \"%1\" at #%2 position.").arg(quote).arg(newQuoteNumber));
    }

    return newQuoteNumber;
}

///////////////////////////////////////////////////////////////////////////

bool QuoteDBHelper::DeleteQuote(int quoteNumber)
{
    bool deleted(false);

    // Check borders
    if ((quoteNumber <= GetMaxQuoteNumber()) && (quoteNumber > 0))
    {
        // Try to get existing quote to delete it later and notify listeners
        DB_QUERY_PTR query = DB_SELECT("Quotes", "Id", QString("Number=%1").arg(quoteNumber));
        if ((query != nullptr) && (query->first()))
        {
            // Try delete quote
            if (DB_DELETE("Quotes", QString("Number=%1").arg(quoteNumber)))
            {
                // Refresh quote numbers
                _RefreshQuoteNumbersFrom(quoteNumber);
                deleted = true;

                // Emit event
                emit QuoteDeleted(query->value("Id").toInt());
            }
            else
            {
                // Log error
                LOG(LogError, this->metaObject()->className(), __func__, QString("Cannot delete the quote #%1.").arg(quoteNumber));
            }
        }
    }

    return deleted;
}

///////////////////////////////////////////////////////////////////////////

bool QuoteDBHelper::EditQuote(int quoteNumber, const QString& newQuote)
{
    bool edited(false);

    // Check borders
    if ((quoteNumber > 0) && (quoteNumber <= GetMaxQuoteNumber()))
    {
        // Try to update quote
        if (DB_UPDATE("Quotes", QString("Quote='%1'").arg(newQuote), QString("Number=%1").arg(quoteNumber)))
        {
            edited = true;

            // Emit event
            emit QuoteUpdated(quoteNumber);
        }
        else
        {
            // Log error
            LOG(LogError, this->metaObject()->className(), __func__, QString("Cannot edited the quote #%1.").arg(quoteNumber));
        }
    }

    return edited;
}

///////////////////////////////////////////////////////////////////////////

QVector<int> QuoteDBHelper::GetQuoteIds()
{
    QVector<int> quoteIds;

    // Get all quote ids
    DB_QUERY_PTR query = DB_SELECT("Quotes", "Id");
    if (query != nullptr)
    {
        while (query->next())
        {
            quoteIds.push_back(query->value("Id").toInt());
        }
    }

    return quoteIds;
}

///////////////////////////////////////////////////////////////////////////

QString QuoteDBHelper::GetQuote(int quoteNumber) const
{
    QString quote;

    // Try to get requested quote
    DB_QUERY_PTR quoteQuery = DB_SELECT("Quotes", "Quote", QString("Number=%1").arg(quoteNumber));
    if ((quoteQuery != nullptr) && (quoteQuery->first()))
    {
        quote = quoteQuery->value("Quote").toString();

    }
    else
    {
        // Log error
        LOG(LogInfo, this->metaObject()->className(), __func__, QString("Quote #%1 does not exist.").arg(quoteNumber));
    }

    return quote;
}

///////////////////////////////////////////////////////////////////////////

QString QuoteDBHelper::GetRandomQuote()
{
    QString quote;

    // Try to get quote
    DB_QUERY_PTR quoteQuery = DB_SELECT("Quotes", "*", "Quote IN (SELECT Quote FROM Quotes ORDER BY RANDOM() LIMIT 1)");
    if ((quoteQuery != nullptr) && (quoteQuery->first()))
    {
        quote = quoteQuery->value("Quote").toString();
        quote.append(" - #" + quoteQuery->value("Number").toString());
    }

    return quote;
}

///////////////////////////////////////////////////////////////////////////

QuoteData QuoteDBHelper::GetQuoteData(int id)
{
    QuoteData quoteData;

    // Try to get requested quote data
    DB_QUERY_PTR query = DB_SELECT("Quotes", "Number, Quote", QString("Id=%1").arg(id));
    if ((query != nullptr) && (query->first()))
    {
        quoteData.QuoteNumber = query->value("Number").toInt();
        quoteData.Quote = query->value("Quote").toString();
    }

    return quoteData;
}

///////////////////////////////////////////////////////////////////////////

void QuoteDBHelper::_RefreshQuoteNumbersFrom(int quoteNumber) const
{
    // Get all quotes where column 'Number' contain bigger value than provided
    DB_QUERY_PTR query = DB_SELECT("Quotes", "Id, Number", QString("Number > %1 ORDER BY Number ASC ").arg(quoteNumber));
    if (query != nullptr)
    {
        // For every quote we need update number value.
        // As this function only used when quote was deleted, that means we should decriment number value and nothing more
        while (query->next())
        {
            int newNumber = query->value("Number").toInt() - 1;
            int id = query->value("id").toInt();
            // If quote number was not updated, log error
            if (!DB_UPDATE("Quotes", QString("Number=%1").arg(newNumber), QString("Id=%1").arg(id)))
            {
                // Log error
                LOG(LogError, "", Q_FUNC_INFO, QString("Cannot update the quote #%1.").arg(newNumber));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
