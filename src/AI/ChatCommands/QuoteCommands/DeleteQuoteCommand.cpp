#include "DeleteQuoteCommand.hpp"
#include <Utils/DatabaseManager.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

DeleteQuoteCommand::DeleteQuoteCommand()
{
    _Clear();
    _name = "!delete_quote";
}

///////////////////////////////////////////////////////////////////////////

QString DeleteQuoteCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    if (message.GetMessage().contains(_name))
    {
        QString msg = message.GetMessage();
        QString val;
        // Try to found number after command
        if (_GetNumberAfterCommand(_name, msg, val))
        {
            // Check borders
            int number = val.toInt();
            std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(number)");
            if (numberQuery != NULL)
            {
                numberQuery->first();
                int maxValue = numberQuery->value(0).toInt() + 1;
                if ((number > 0) && (number <= maxValue))
                {
                    if(DB_DELETE("Quotes", QString("number = %1").arg(number)))
                    {
                        _RefreshQuoteNumbers(number);
                        answer = "Quote #";
                        answer.append(QString::number(number));
                        answer.append(" was removed!");
                    }
                }
            }
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

void DeleteQuoteCommand::_RefreshQuoteNumbers(int quoteNumber)
{
    std::shared_ptr<QSqlQuery> query = DB_SELECT("Quotes", "id, number", QString("number > %1 ORDER BY number ASC ").arg(quoteNumber));
    if (query != NULL)
    {
        while (query->next())
        {
            int newNumber = query->value("number").toInt() - 1;
            int id = query->value("id").toInt();
            DB_UPDATE("Quotes", QString("number = %1").arg(newNumber), QString("id = %1").arg(id));
        }
    }
}

///////////////////////////////////////////////////////////////////////////
