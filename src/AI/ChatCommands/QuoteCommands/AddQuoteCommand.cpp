/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AddQuoteCommand.hpp"
#include <Utils/DatabaseManager.hpp>

using namespace Command;

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

void AddQuoteCommand::_GetAnswer(const ChatMessage& message, QStringList& answer)
{
    if (_CheckModerationFlag(message.IsModerator()))
    {
        QString msg = message.GetMessage();
        // Get quote that should ba added
        int id = msg.indexOf(_name);
        msg = msg.mid(id + _name.size() + 1);
        msg = msg.left(msg.size() - 2);
        // If message not empty add quote to the list
        if (!msg.isEmpty())
        {
            std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(Number)");
            if (numberQuery != NULL)
            {
                numberQuery->first();
                int newMaxValue = numberQuery->value(0).toInt() + 1;

                if (DB_INSERT("Quotes", QString("NULL, '%1', %2").arg(msg).arg(newMaxValue)))
                {
                    answer.append(_answers.at(0));
                    (*answer.begin()).replace("QUOTE_NUMBER", QString::number(newMaxValue));
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void AddQuoteCommand::_GetRandomAnswer(const ChatMessage& message, QStringList& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
