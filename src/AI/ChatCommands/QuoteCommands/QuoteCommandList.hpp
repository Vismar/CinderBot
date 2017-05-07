#pragma once
#include "../CommandList.hpp"

namespace Command
{

/*!
 * Class QuoteCommandList
 * List of quote commands
 */
class QuoteCommandList : public CommandList
{
public:
    /*! Constructor */
    QuoteCommandList();
    /*! Destructor */
    ~QuoteCommandList();

protected:

    ////////////////////////////////
    /// CommandList override
    void _Initialize();
};

}
