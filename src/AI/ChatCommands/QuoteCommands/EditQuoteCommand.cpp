#include "EditQuoteCommand.hpp"

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
            int number = val.toInt() - 1;
            if ((number >= 0) && (number < _quotes->size()))
            {
                // Remove part with number from message
                msg = msg.mid(msg.indexOf(_name) + _name.size() + 1);
                msg = msg.left(msg.size() - 2);
                msg = msg.mid(val.size() + 1);
                // Update quote
                (*_quotes)[number] = msg;
                answer = "Quote #";
                answer.append(QString::number(number + 1));
                answer.append(" was edited!");
            }
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
