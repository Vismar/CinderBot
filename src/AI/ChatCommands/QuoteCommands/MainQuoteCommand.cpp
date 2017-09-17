/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MainQuoteCommand.hpp"
#include "Utils/Database/QuoteDBHelper.hpp"

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
    QString quote;
    
    // Try to found number after command
    if (_GetNumberAfterCommand(message.GetMessage(), quote))
    {
        int number = quote.toInt();
        quote = QuoteDBHelper::Instance().GetQuote(number);
        if (!quote.isEmpty())
        {
            quote.append(" - #" + QString::number(number));
        }
    }

    // If quote was not acquired, get random quote
    if (quote.isEmpty())
    {
        quote = QuoteDBHelper::Instance().GetRandomQuote();
    }

    if (!quote.isEmpty())
    {
        answer.AddAnswer(quote);
    }
}

///////////////////////////////////////////////////////////////////////////

void MainQuoteCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
