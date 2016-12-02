#pragma once
#include "CommandList.hpp"

namespace Command
{

/*!
 * Class CustomCommandList
 * Read and store all custom commands from .xml file
 */
class CustomCommandList : public CommandList
{
public:
    /*! Constructor */
    CustomCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void _Initialize();
};

}
