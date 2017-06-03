/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <AI/ChatCommands/CommandList.hpp>

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
