#include "MainQuoteCommand.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

MainQuoteCommand::MainQuoteCommand()
{
    _Clear();
    _name = "#quote";
}

///////////////////////////////////////////////////////////////////////////

QString MainQuoteCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    QString msg = message.GetMessage();
    QString val;
    if (message.GetMessage().contains(_name))
    {
        // Try to found number after command
        if (_GetNumberAfterCommand(_name, msg, val))
        {
            // Check borders
            int number = val.toInt() - 1;
            if ((number >= 0) && (number < _quotes->size()))
            {
                answer = _quotes->at(number);
                answer.append(" - #" + QString::number(number + 1));
            }
        }
        // If check failed return random quote
        if (answer.isEmpty())
        {
            int k = qrand() % _quotes->size();
            answer = _quotes->at(k);
            answer.append(" - #" + QString::number(k + 1));
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
