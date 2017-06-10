/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
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
