#include "CustomCommandList.hpp"
#include <QFile>
#include <QXmlStreamReader>
#include "../../Utils/Config/ConfigurationManager.hpp"
#include "../../Utils/Config/ConfigurationParameters.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CustomCommandList::CustomCommandList()
{
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_Initialize()
{
    // Custom commands
    _ReadXml("./data/config/Commands.xml");
    // Covenant commands
    ConfigurationManager& configMng = ConfigurationManager::Instance();
    QString covenantListString;
    configMng.GetStringParam(CFGS_COVENANTS, covenantListString);
    QStringList covenantList = covenantListString.split(",");
    for (int i = 0; i < covenantList.size(); ++i)
    {
        covenantListString = QString("./data/config/%1.xml").arg(covenantList[i]);
        _ReadXml(covenantListString);
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandList::_ReadXml(const QString& filePath)
{
    QXmlStreamReader xmlReader;
    // Should be changed when global strings will be implemented
    QFile commandsFile(filePath);
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
