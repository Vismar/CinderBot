/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CovenantCommandList.hpp"
#include <AI/ChatCommands/CovenantCommands/ListCovenantCommand.hpp>
#include <AI/ChatCommands/CovenantCommands/JoinCovenantCommand.hpp>
#include <AI/ChatCommands/CovenantCommands/LeaveCovenantCommand.hpp>
#include <AI/ChatCommands/CovenantCommands/CreateCovenantCommand.hpp>
#include <AI/ChatCommands/CovenantCommands/RenameCovenantCommand.hpp>
#include <AI/ChatCommands/CovenantCommands/DescriptionCovenantCommand.hpp>
#include <AI/ChatCommands/CovenantCommands/InfoCovenantCommand.hpp>
#include <AI/ChatCommands/CovenantCommands/DisbandCovenantCommand.hpp>
#include <Utils/DatabaseManager.hpp>

using namespace Command;

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
    _commands.push_back(new DisbandCovenantCommand());

    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CovenantCommandList::_Initialize()
{
    // Initialize data table in database
    DB_CREATE_TABLE("Covenants", "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Name TEXT NOT NULL UNIQUE,"
                                 "Leader TEXT,"
                                 "Description TEXT,"
                                 "CmdSlots INTEGER");
}

///////////////////////////////////////////////////////////////////////////
