#pragma once
#include "CommandList.hpp"

namespace Command
{

class UserDataCommandList : public CommandList
{
public:
    /*! Constructor */
    UserDataCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void _Initialize();
};

}
