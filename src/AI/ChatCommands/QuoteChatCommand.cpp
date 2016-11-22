#include "QuoteChatCommand.hpp"
#include <QFile>
#include <QStringList>
#include <QtGlobal>
#include <QTime>

#include <QDebug>

#define QCC_FILE_NAME      "./data/data/Quotes.xml"
#define QCC_SECTION_QUOTE  "Quote"

#define QCC_COMMAND_MAIN   "#quote"
#define QCC_COMMAND_ADD    "#add_quote"
#define QCC_COMMAND_DELETE "#delete_quote"
#define QCC_COMMAND_EDIT   "#edit_quote"
#define QCC_COMMAND_ID     "#"

///////////////////////////////////////////////////////////////////////////

void QuoteChatCommand::_Initialize()
{
    QFile commandsFile(QCC_FILE_NAME);
    // Try to open file
    if (commandsFile.open(QIODevice::ReadOnly))
    {
        _xmlReader.setDevice(&commandsFile);

        // Read xml file
        while (!_xmlReader.atEnd())
        {
            _xmlReader.readNext();
            if (_xmlReader.isStartElement())
            {
                // If it is start section of quote, read it
                if (_xmlReader.name() == QCC_SECTION_QUOTE)
                {
                    QString quote = _xmlReader.readElementText();
                    _quotes.push_back(quote);
                }
            }
        }
    }
    commandsFile.close();
}

///////////////////////////////////////////////////////////////////////////

void QuoteChatCommand::_SaveQuotes()
{
    for (int i = 0; i < _quotes.size(); ++i)
    {
        _xmlWriter.writeTextElement("Quote", _quotes.at(i));
    }
}

///////////////////////////////////////////////////////////////////////////

bool QuoteChatCommand::_GetNumberAfterCommand(const QString& command, const QString& message, QString& value)
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

QuoteChatCommand::QuoteChatCommand()
{
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

QuoteChatCommand::~QuoteChatCommand()
{
    QFile userDataFile(QCC_FILE_NAME);
    if (userDataFile.open(QIODevice::WriteOnly))
    {
        _xmlWriter.setDevice(&userDataFile);
        _xmlWriter.setAutoFormatting(true);
        _xmlWriter.writeStartDocument();
        _xmlWriter.writeStartElement("Quotes");
        _SaveQuotes();
        _xmlWriter.writeEndElement();
        _xmlWriter.writeEndDocument();
    }
    userDataFile.close();
}

///////////////////////////////////////////////////////////////////////////

bool QuoteChatCommand::GetAnswer(ChatMessage &message, QString &answer)
{
    bool result(false);
    QString msg = message.GetMessage();
    QString val;
    if (msg.contains(QCC_COMMAND_MAIN))
    {
        // Try to found number after command
        if (_GetNumberAfterCommand(QCC_COMMAND_MAIN, msg, val))
        {
            // Check borders
            int number = val.toInt() - 1;
            if ((number >= 0) && (number < _quotes.size()))
            {
                result = true;
                answer = _quotes.at(number);
            }
            // If check failed return random quote
            if (!result)
            {
                result = true;
                int k = qrand() % _quotes.size();
                answer = _quotes.at(k);
            }
        }
        // If number is not exist
        else
        {
            result = true;
            int k = qrand() % _quotes.size();
            answer = _quotes.at(k);
        }
    }
    else if (msg.contains(QCC_COMMAND_ADD) && (message.IsModerator()))
    {
        // Get quote that should ba added
        int id = msg.indexOf(QCC_COMMAND_ADD);
        msg = msg.mid(id + QString(QCC_COMMAND_ADD).size() + 1);
        msg = msg.left(msg.size() - 2);
        // If message not empty add quote to the list
        if (!msg.isEmpty())
        {
            _quotes.push_back(msg);
            result = true;
            answer = "Quote #";
            answer.append(QString::number(_quotes.size()));
            answer.append(" was added!");
        }
    }
    else if ((msg.contains(QCC_COMMAND_DELETE)) && (message.IsModerator()))
    {
        // Try to found number after command
        if (_GetNumberAfterCommand(QCC_COMMAND_DELETE, msg, val))
        {
            // Check borders
            int number = val.toInt() - 1;
            if ((number >= 0) && (number < _quotes.size()))
            {
                result = true;
                _quotes.remove(number);
                answer = "Quote #";
                answer.append(QString::number(number + 1));
                answer.append(" was removed!");
            }
        }
    }
    else if ((msg.contains(QCC_COMMAND_EDIT)) && (message.IsModerator()))
    {
        // Try to found number after command
        if (_GetNumberAfterCommand(QCC_COMMAND_EDIT, msg, val))
        {
            // Check borders
            int number = val.toInt() - 1;
            if ((number >= 0) && (number < _quotes.size()))
            {
                result = true;
                // Remove part with number from message
                msg = msg.mid(msg.indexOf(QCC_COMMAND_EDIT) + QString(QCC_COMMAND_EDIT).size() + 1);
                msg = msg.left(msg.size() - 2);
                msg = msg.mid(val.size() + 1);
                // Update quote
                _quotes[number] = msg;
                answer = "Quote #";
                answer.append(QString::number(number + 1));
                answer.append(" was edited!");
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
