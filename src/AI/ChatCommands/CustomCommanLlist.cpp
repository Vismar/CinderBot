/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCommandList.hpp"
#include <QFile>
#include <QXmlStreamReader>
#include <Utils/Config/ConfigurationManager.hpp>
#include <Utils/Config/ConfigurationParameters.hpp>
#include <Utils/DatabaseManager.hpp>

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
    QStringList covList;
    std::shared_ptr<QSqlQuery> query = DB_SELECT("Covenants", "Name");
    if (query->exec())
    {
        while (query->next())
        {
            covList.append(query->value("Name").toString());
        }
    }
    for (int i = 0; i < covList.size(); ++i)
    {
        QString covenantCmdPath = QString("./data/config/%1.xml").arg(covList[i]);
        _ReadXml(covenantCmdPath);
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
