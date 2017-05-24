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
    _name = "!add_quote";
}

///////////////////////////////////////////////////////////////////////////

QString AddQuoteCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    if (message.GetMessage().contains(_name))
    {
        QString msg = message.GetMessage();
        // Get quote that should ba added
        int id = msg.indexOf(_name);
        msg = msg.mid(id + _name.size() + 1);
        msg = msg.left(msg.size() - 2);
        // If message not empty add quote to the list
        if (!msg.isEmpty())
        {
            std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(number)");
            if (numberQuery != NULL)
            {
                numberQuery->first();
                int newMaxValue = numberQuery->value(0).toInt() + 1;

                if (DB_INSERT("Quotes", QString("NULL, '%1', %2").arg(msg).arg(newMaxValue)))
                {
                    answer = "Quote #";
                    answer.append(QString::number(newMaxValue));
                    answer.append(" was added!");
                }
            }
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
