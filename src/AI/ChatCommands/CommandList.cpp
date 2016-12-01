#include "CommandList.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CommandList::CommandList()
{
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

CommandList::~CommandList() { }

///////////////////////////////////////////////////////////////////////////

bool CommandList::TryGetAnswer(const ChatMessage& message, QString& answer)
{
    bool result(false);
    // Check all commands
    for (int i = 0; i < _commands.size(); ++i)
    {
        answer = _commands[i].GetRandomAnswer(message);
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
