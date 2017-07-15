/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CommandList.hpp"

namespace Command
{
namespace QuoteCmd
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
    void Initialize();
};

}
}
