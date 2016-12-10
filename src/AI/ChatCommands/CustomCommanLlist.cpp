#include "CustomCommandList.hpp"
#include <QFile>
#include <QXmlStreamReader>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CustomCommandList::CustomCommandList()
{
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_Initialize()
{
    QXmlStreamReader xmlReader;
    // Should be changed when global strings will be implemented
    QFile commandsFile("./data/config/Commands.xml");
    // Try to open file
    if (commandsFile.open(QIODevice::ReadOnly))
    {
        xmlReader.setDevice(&commandsFile);
        // Read xml file
        while (!xmlReader.atEnd())
        {
            xmlReader.readNext();
            if (xmlReader.isStartElement())
            {
                // If it is start section of command, read it
                if (xmlReader.name() == ChatCommand::GetSectionName(Start))
                {
                    ChatCommand* command = new ChatCommand();
                    if (command->Initialize(xmlReader))
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
