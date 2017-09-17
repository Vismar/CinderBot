/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "DeleteQuoteCommand.hpp"
#include "Utils/Database/QuoteDBHelper.hpp"

using namespace Command::QuoteCmd;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

DeleteQuoteCommand::DeleteQuoteCommand()
{
    _Clear();
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void DeleteQuoteCommand::Initialize()
{
    _name = "!quote_delete";
    _moderatorOnly = true;
    _answers.push_back("Quote #QUOTE_NUMBER was deleted!");
}

///////////////////////////////////////////////////////////////////////////

void DeleteQuoteCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    if (_CheckModerationFlag(message.IsModerator()))
    {
        QString val;
        // Try to found number after command
        if (_GetNumberAfterCommand(message.GetMessage(), val))
        {
            // Check borders
            int number = val.toInt();
            if (QuoteDBHelper::Instance().DeleteQuote(number))
            {
                answer.AddAnswer(_answers.at(0));
                (*answer.GetAnswers().begin()).replace("QUOTE_NUMBER", QString::number(number));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void DeleteQuoteCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
