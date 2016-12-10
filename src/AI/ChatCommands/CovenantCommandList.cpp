#include "CovenantCommandList.hpp"
#include "CovenantCommand.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CovenantCommandList::CovenantCommandList()
{
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CovenantCommandList::_Initialize()
{
    for (int i = Cov_List; i < Cov_End; ++i)
    {
        CovenantCommand* command = new CovenantCommand();
        command->SetCommandType(static_cast<CovCommandType>(i));
        _commands.push_back(command);
    }
}

///////////////////////////////////////////////////////////////////////////
