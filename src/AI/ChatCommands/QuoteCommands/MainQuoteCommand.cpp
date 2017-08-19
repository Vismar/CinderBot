/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MainQuoteCommand.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Command::QuoteCmd;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

MainQuoteCommand::MainQuoteCommand()
{
    _Clear();
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void MainQuoteCommand::Initialize()
{
    _name = "!quote";
}

///////////////////////////////////////////////////////////////////////////

void MainQuoteCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    QString val;
    DB_QUERY_PTR numberQuery = DB_SELECT("Quotes", "MAX(Number)");
    if (numberQuery != NULL)
    {
        numberQuery->first();
        int maxValue = numberQuery->value(0).toInt();

        // Try to found number after command
        if (_GetNumberAfterCommand(_name, message.GetMessage(), val))
        {
            // Check borders
            int number = val.toInt();
            if ((number > 0) && (number <= maxValue))
            {
                DB_QUERY_PTR query = DB_SELECT("Quotes", "Quote", QString("Number = %1").arg(number));
                if (query != NULL)
                {
                    query->first();
                    answer.AddAnswer(query->value("quote").toString());
                    (*answer.GetAnswers().begin()).append(" - #" + QString::number(number));
                }
            }
        }
        // If check failed return random quote
        if (answer.GetAnswers().isEmpty())
        {
            int k = qrand() % maxValue;
            DB_QUERY_PTR query = DB_SELECT("Quotes", "Quote", QString("Number = %1").arg(k+1));
            if (query != NULL)
            {
                if (query->first())
                {
                    answer.AddAnswer(query->value("Quote").toString());
                    (*answer.GetAnswers().begin()).append(" - #" + QString::number(k+1));
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void MainQuoteCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
