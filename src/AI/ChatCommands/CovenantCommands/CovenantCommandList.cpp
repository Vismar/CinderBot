/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CovenantCommandList.hpp"
#include "AI/ChatCommands/CovenantCommands/ListCovenantCommand.hpp"
#include "AI/ChatCommands/CovenantCommands/JoinCovenantCommand.hpp"
#include "AI/ChatCommands/CovenantCommands/LeaveCovenantCommand.hpp"
#include "AI/ChatCommands/CovenantCommands/CreateCovenantCommand.hpp"
#include "AI/ChatCommands/CovenantCommands/RenameCovenantCommand.hpp"
#include "AI/ChatCommands/CovenantCommands/DescriptionCovenantCommand.hpp"
#include "AI/ChatCommands/CovenantCommands/InfoCovenantCommand.hpp"
#include "AI/ChatCommands/CovenantCommands/MembersCovenantCommand.hpp"
#include "AI/ChatCommands/CovenantCommands/DisbandCovenantCommand.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Config/ConfigurationManager.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CovenantCommandList::CovenantCommandList()
{
    // Add all commands
    _commands.push_back(new ListCovenantCommand());
    _commands.push_back(new JoinCovenantCommand());
    _commands.push_back(new LeaveCovenantCommand());
    _commands.push_back(new CreateCovenantCommand());
    _commands.push_back(new RenameCovenantCommand());
    _commands.push_back(new DescriptionCovenantCommand());
    _commands.push_back(new InfoCovenantCommand());
    _commands.push_back(new MembersCovenantCommand());
    _commands.push_back(new DisbandCovenantCommand());

    Initialize();
    OnCfgParamChanged(CfgParam::CovenantCmdModule);
}

///////////////////////////////////////////////////////////////////////////

void CovenantCommandList::Initialize()
{
    // Initialize data table in database
    DB_CREATE_TABLE("Covenants", "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Name TEXT NOT NULL UNIQUE,"
                                 "Leader TEXT NOT NULL,"
                                 "Description TEXT NOT NULL,"
                                 "CmdSlots INTEGER NOT NULL,"
                                 "Level INTEGER NOT NULL,"
                                 "Exp INTEGER NOT NULL,"
                                 "MaxMembers INTEGER DEFAULT 10");
}

///////////////////////////////////////////////////////////////////////////

void CovenantCommandList::OnCfgParamChanged(CfgParam cfgParam)
{
    QString value;
    switch (cfgParam)
    {
    case CfgParam::CovenantCmdModule:
        ConfigurationManager::Instance().GetStringParam(CfgParam::CovenantCmdModule, value);
        _isTurnedOn = ("true" == value);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
