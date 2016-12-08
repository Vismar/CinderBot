#pragma once
#include "CommandList.hpp"

namespace Command
{

/*!
 * Class CovenantCommandList
 * Store all covenant commands
 */
class CovenantCommandList : public CommandList
{
public:
    /*! Constructor */
    CovenantCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void _Initialize();
};

}
