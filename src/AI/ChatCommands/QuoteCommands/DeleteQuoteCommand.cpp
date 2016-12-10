#include "DeleteQuoteCommand.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

DeleteQuoteCommand::DeleteQuoteCommand()
{
    _Clear();
    _name = "#delete_quote";
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
            int number = val.toInt() - 1;
            if ((number >= 0) && (number < _quotes->size()))
            {
                _quotes->remove(number);
                answer = "Quote #";
                answer.append(QString::number(number + 1));
                answer.append(" was removed!");
            }
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
