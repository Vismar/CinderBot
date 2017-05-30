/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CommandList.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CommandList::~CommandList()
{
    for (int i = 0; i < _commands.size(); ++i)
    {
        delete _commands[i];
    }
}

///////////////////////////////////////////////////////////////////////////

bool CommandList::TryExecute(const ChatMessage& message, QString& answer)
{
    bool result(false);
    // Check all commands
    for (int i = 0; i < _commands.size(); ++i)
    {
        answer = _commands[i]->GetRandomAnswer(message);
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

void CommandList::_Initialize() { }

///////////////////////////////////////////////////////////////////////////
