/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CommandList.hpp"
#include "Utils/Config/ConfigurationManager.hpp"

using namespace Utils::Configuration;
using namespace Command;

///////////////////////////////////////////////////////////////////////////

CommandList::CommandList()
{
    _isTurnedOn = true;
    connect(&ConfigurationManager::Instance(), &ConfigurationManager::ParameterChanged,
            this, &CommandList::OnCfgParamChanged);
}

///////////////////////////////////////////////////////////////////////////

CommandList::~CommandList()
{
    for (int i = 0; i < _commands.size(); ++i)
    {
        delete _commands[i];
    }
}

///////////////////////////////////////////////////////////////////////////

bool CommandList::TryExecute(const ChatMessage &message, ChatAnswer &answer)
{
    bool result(false);
    // If module turned on, try to execute commands
    if (_isTurnedOn)
    {
        // Check all commands
        for (int i = 0; i < _commands.size(); ++i)
        {
            if (_commands[i]->Execute(message, answer))
            {
                result = true;
                break;
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
