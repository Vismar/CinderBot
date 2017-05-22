#include "MainQuoteCommand.hpp"
#include <Utils/DatabaseManager.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

MainQuoteCommand::MainQuoteCommand()
{
    _Clear();
    _name = "!quote";
}

///////////////////////////////////////////////////////////////////////////

QString MainQuoteCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    QString msg = message.GetMessage();
    QString val;
    if (message.GetMessage().contains(_name))
    {
        std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(number)");
        if (numberQuery != NULL)
        {
            numberQuery->first();
            int maxValue = numberQuery->value(0).toInt();

            // Try to found number after command
            if (_GetNumberAfterCommand(_name, msg, val))
            {
                // Check borders
                int number = val.toInt();
                if ((number > 0) && (number <= maxValue))
                {
                    std::shared_ptr<QSqlQuery> query = DB_SELECT("Quotes", "quote", QString("number = %1").arg(number));
                    if (query != NULL)
                    {
                        query->first();
                        answer = query->value(0).toString();
                        answer.append(" - #" + QString::number(number));
                    }
                }
            }
            // If check failed return random quote
            if (answer.isEmpty())
            {
                int k = qrand() % maxValue;
                std::shared_ptr<QSqlQuery> query = DB_SELECT("Quotes", "quote", QString("number = %1").arg(k+1));
                if (query != NULL)
                {
                    query->first();
                    answer = query->value(0).toString();
                    answer.append(" - #" + QString::number(k+1));
                }
            }
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
