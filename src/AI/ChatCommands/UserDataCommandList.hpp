#pragma once
#include "CommandList.hpp"

namespace Command
{

/*!
 * Class UserDataCommandList
 * Store all UD commands
 */
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
