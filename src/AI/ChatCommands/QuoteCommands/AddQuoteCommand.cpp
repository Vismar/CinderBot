#include "AddQuoteCommand.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

AddQuoteCommand::AddQuoteCommand()
{
    _Clear();
    _name = "#add_quote";
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
            _quotes->push_back(msg);
            answer = "Quote #";
            answer.append(QString::number(_quotes->size()));
            answer.append(" was added!");
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
