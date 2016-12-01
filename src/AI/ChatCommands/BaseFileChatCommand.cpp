#include "BaseFileChatCommand.hpp"
#include <QFile>
#include <QTime>

#define BFCC_FILE_NAME        "./data/config/commands.xml"

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
                if (_xmlReader.name() == ChatCommand::GetSectionName(Start))
                {
                    ChatCommand command;
                    if (command.Initialize(_xmlReader))
                    {
                        _commands.push_back(command);
                    }
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
