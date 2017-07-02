/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "DeleteQuoteCommand.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

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
    _answers.push_back("Quote #QUOTE_NUMBER was removed!");
}

///////////////////////////////////////////////////////////////////////////

void DeleteQuoteCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    if (_CheckModerationFlag(message.IsModerator()))
    {
        QString val;
        // Try to found number after command
        if (_GetNumberAfterCommand(_name, message.GetMessage(), val))
        {
            // Check borders
            int number = val.toInt();
            std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(Number)");
            if (numberQuery != NULL)
            {
                numberQuery->first();
                int maxValue = numberQuery->value(0).toInt() + 1;
                if ((number > 0) && (number <= maxValue))
                {
                    if(DB_DELETE("Quotes", QString("Number = %1").arg(number)))
                    {
                        _RefreshQuoteNumbers(number);
                        answer.AddAnswer(_answers.at(0));
                        (*answer.GetAnswers().begin()).replace("QUOTE_NUMBER", QString::number(number));
                    }
                }
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

void DeleteQuoteCommand::_RefreshQuoteNumbers(int quoteNumber)
{
    std::shared_ptr<QSqlQuery> query = DB_SELECT("Quotes",
                                                 "Id, Number",
                                                 QString("Number > %1 ORDER BY Number ASC ").arg(quoteNumber));
    if (query != NULL)
    {
        while (query->next())
        {
            int newNumber = query->value("Number").toInt() - 1;
            int id = query->value("id").toInt();
            DB_UPDATE("Quotes", QString("Number = %1").arg(newNumber), QString("Id = %1").arg(id));
        }
    }
}

///////////////////////////////////////////////////////////////////////////
