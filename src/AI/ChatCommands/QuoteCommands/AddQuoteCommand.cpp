/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AddQuoteCommand.hpp"
#include "Utils/Database/QuoteDBHelper.hpp"

using namespace Command::QuoteCmd;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

AddQuoteCommand::AddQuoteCommand()
{
    _Clear();
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void AddQuoteCommand::Initialize()
{
    _name = "!quote_add";
    _moderatorOnly = true;
    _answers.push_back("Quote #QUOTE_NUMBER was added!");
}

///////////////////////////////////////////////////////////////////////////

void AddQuoteCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    if (_CheckModerationFlag(message.IsModerator()))
    {        
        // Get quote that should be added
        int id = message.GetMessage().indexOf(_name);
        QString msg = message.GetMessage().right(message.GetMessage().size() - id - _name.size() - 1);
        // If message not empty add quote to the list
        if (!msg.isEmpty())
        {
            int quoteNumber = QuoteDBHelper::Instance().AddQuote(msg);

            answer.AddAnswer(_answers.at(0));
            (*answer.GetAnswers().begin()).replace("QUOTE_NUMBER", QString::number(quoteNumber));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void AddQuoteCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
