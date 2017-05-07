#include "EditQuoteCommand.hpp"
#include <Utils/DatabaseManager.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

EditQuoteCommand::EditQuoteCommand()
{
    _Clear();
    _name = "#edit_quote";
}

///////////////////////////////////////////////////////////////////////////

QString EditQuoteCommand::GetRandomAnswer(const ChatMessage& message)
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
                    // Remove part with number from message
                    msg = msg.mid(msg.indexOf(_name) + _name.size() + 1);
                    msg = msg.left(msg.size() - 2);
                    msg = msg.mid(val.size() + 1);
                    // Update quote
                    if(DB_UPDATE("Quotes", QString("quote = '%1'").arg(msg), QString("number = %1").arg(number)))
                    {
                        answer = "Quote #";
                        answer.append(QString::number(number));
                        answer.append(" was edited!");
                    }
                }
            }

        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
