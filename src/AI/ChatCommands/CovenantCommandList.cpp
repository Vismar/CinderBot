#include "CovenantCommandList.hpp"
#include "CovenantCommand.hpp"
#include <Utils/DatabaseManager.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CovenantCommandList::CovenantCommandList()
{
    for (int i = Cov_List; i < Cov_End; ++i)
    {
        CovenantCommand* command = new CovenantCommand();
        command->SetCommandType(static_cast<CovCommandType>(i));
        _commands.push_back(command);
    }

    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CovenantCommandList::_Initialize()
{
    // Initialize data table in database
    DB_CREATE_TABLE("Covenants", "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Name TEXT NOT NULL UNIQUE");
}

///////////////////////////////////////////////////////////////////////////
