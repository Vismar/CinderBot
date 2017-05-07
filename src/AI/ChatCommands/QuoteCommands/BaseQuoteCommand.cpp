#include "BaseQuoteCommand.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

BaseQuoteCommand::BaseQuoteCommand() {}

///////////////////////////////////////////////////////////////////////////

bool BaseQuoteCommand::_GetNumberAfterCommand(const QString &command, const QString &message, QString &value)
{
    bool isDigit(false);
    QString msg = message;
    // Get start of a command
    int id = msg.indexOf(command);
    if (id != -1)
    {
        // Check size
        if ((id + QString(command).size() + 2) < message.size())
        {
            isDigit = true;
            // Get string that should contain number
            msg = msg.mid(id + QString(command).size() + 1);
            msg = msg.left(msg.size() - 2);
            msg.append(" ");
            QStringList words = msg.split(" ");
            value = words.first();
            // Check if it is number
            for (int i = 0; i < value.size(); ++i)
            {
                if (!value.at(i).isDigit())
                {
                    isDigit = false;
                }
            }
        }
    }

    return isDigit;
}

///////////////////////////////////////////////////////////////////////////
