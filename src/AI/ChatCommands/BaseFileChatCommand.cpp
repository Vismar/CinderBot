#include "BaseFileChatCommand.hpp"
#include <QFile>
#include <QTime>

#define BFCC_FILE_NAME        "./data/config/commands.xml"
#define BFCC_SECTION_COMMAND  "Command"
#define BFCC_SECTION_NAME     "Name"
#define BFCC_SECTION_ANSWER   "Answer"
#define BFCC_SECTION_COOLDOWN "Cooldown"
#define BFCC_SYMBOL_AUTHOR    "@"

///////////////////////////////////////////////////////////////////////////

BaseFileChatCommand::BaseFileChatCommand()
{
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

void BaseFileChatCommand::_Initialize()
{
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
                // If it is start section of command, read it
                if (_xmlReader.name() == BFCC_SECTION_COMMAND)
                {
                    _ReadCommand();
                }
            }
        }
    }
    commandsFile.close();
}

///////////////////////////////////////////////////////////////////////////

bool BaseFileChatCommand::GetAnswer(const ChatMessage& message, QString& answer)
{
    bool result(false);
    // Check all commands
    for (int i = 0; i < _commands.size(); ++i)
    {
        answer = _commands[i].GetRandomAnswer(message);
        // If answer is empty, that means command not executed
        if (!answer.isEmpty())
        {
            result = true;
            break;
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void BaseFileChatCommand::_ReadCommand()
{
    QString name;
    QTime cooldown;
    cooldown.setHMS(0, 0, 0, 0);
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
            // If we found a cooldown section for the first time, save it
            else if (_xmlReader.name() == BFCC_SECTION_COOLDOWN)
            {
                QString textCooldown = _xmlReader.readElementText();
                cooldown = QTime::fromString(textCooldown, "h:m:s:z");
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
        BFChatCommand newChatCommand;
        newChatCommand.InitializeCommand(name, answers, cooldown);
        _commands.push_back(newChatCommand);
    }
}

///////////////////////////////////////////////////////////////////////////
