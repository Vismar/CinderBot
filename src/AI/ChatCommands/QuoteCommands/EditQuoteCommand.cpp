/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EditQuoteCommand.hpp"
#include "Utils/Database/QuoteDBHelper.hpp"

using namespace Command::QuoteCmd;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

EditQuoteCommand::EditQuoteCommand()
{
    _Clear();
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void EditQuoteCommand::Initialize()
{
    _name = "!quote_edit";
    _moderatorOnly = true;
    _answers.push_back("Quote #QUOTE_NUMBER was edited!");
}

///////////////////////////////////////////////////////////////////////////

void EditQuoteCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    if (_CheckModerationFlag(message.IsModerator()))
    {
        QString val;
        // Try to found number after command
        if (_GetNumberAfterCommand(message.GetMessage(), val))
        {
            // Check borders
            int number = val.toInt();
            const QString &refMsg = message.GetMessage();
            QString msg = refMsg.right(refMsg.size() - refMsg.indexOf(_name) - _name.size() - val.size() - 2);
            if (QuoteDBHelper::Instance().EditQuote(number, msg))
            {
                answer.AddAnswer(_answers.at(0));
                (*answer.GetAnswers().begin()).replace("QUOTE_NUMBER", QString::number(number));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void EditQuoteCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
