#include "BaseFileChatCommand.hpp"
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QTime>

#define BFCC_FILE_NAME       "./data/config/commands.xml"
#define BFCC_SECTION_COMMAND "Command"
#define BFCC_SECTION_NAME    "Name"
#define BFCC_SECTION_ANSWER  "Answer"
#define BFCC_SYMBOL_AUTHOR   "@"

///////////////////////////////////////////////////////////////////////////

bool BaseFileChatCommand::Initialize()
{
    bool result(true);
    QFile commandsFile(BFCC_FILE_NAME);
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
                // If it is start section of cammand, read it
                if (_xmlReader.name() == BFCC_SECTION_COMMAND)
                {
                    _ReadCommand();
                }
            }
        }
    }
    else
    {
        result = false;
    }
    commandsFile.close();

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool BaseFileChatCommand::GetAnswer(ChatMessage& message, QString& answer)
{
    bool result(false);
    for (int i = 0; i < _commands.size(); ++i)
    {
        // If message contains commnd from list, execute it
        if (message.GetMessage().contains(_commands.at(i).first))
        {
            result = true;
            // Pick random answer
            int k = qrand() % _commands.at(i).second.size();
            answer = _commands.at(i).second.at(k);
            // if answer contains special symbol, replace with username, who typed this command
            if (answer.contains(BFCC_SYMBOL_AUTHOR))
            {
                answer.insert(answer.indexOf(BFCC_SYMBOL_AUTHOR) + 1,
                              message.GetAuthor());
            }
            break;
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void BaseFileChatCommand::_ReadCommand()
{
    QString name;
    QVector<QString> answers;
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        // If end of command section was founded, break loop
        if (_xmlReader.isEndElement())
        {
            if (_xmlReader.name() == BFCC_SECTION_COMMAND)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            // If we found a command name section for the first time, save it
            if((_xmlReader.name() == BFCC_SECTION_NAME) && (name.isEmpty()))
            {
                name = _xmlReader.readElementText();
            }
            // If we found a answer section, should try to get text
            else if (_xmlReader.name() == BFCC_SECTION_ANSWER)
            {
                // Try to get text
                QString answer = _xmlReader.readElementText();
                // Remove "new line" and "tab" symbols
                answer.replace("\n", "");
                answer.replace("\t", "");
                // If result text is not empty, add to answers list
                if (!answer.isEmpty())
                {
                    answers.push_back(answer);
                }
            }
        }
    }
    // If name of command and possible answer(or answers) were found, add it to commands list
    if (!name.isEmpty() && !answers.isEmpty())
    {
        _commands.push_back(QPair<QString, QVector<QString> >(name, answers));
    }
}

///////////////////////////////////////////////////////////////////////////
