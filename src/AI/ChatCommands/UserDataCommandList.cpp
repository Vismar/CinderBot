#include "UserDataCommandList.hpp"
#include "UserDataCommand.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

UserDataCommandList::UserDataCommandList()
{
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

void UserDataCommandList::_Initialize()
{
    for (int i = UDC_Messages; i < UDC_End; ++i)
    {
        UserDataCommand* command = new UserDataCommand();
        command->SetCommandType(static_cast<UDCommandType>(i));
        _commands.push_back(command);
    }
}

///////////////////////////////////////////////////////////////////////////
