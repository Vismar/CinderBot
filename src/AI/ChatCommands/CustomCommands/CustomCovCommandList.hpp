/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CustomCommands/CustomCommandList.hpp"

namespace Command
{

/*!
 * Class CustomCovCommandList
 * Read and store all custom commands for covenants from database
 */
class CustomCovCommandList : public CustomCommandList
{
public:
    /*! Constructor */
    CustomCovCommandList();

protected:
    ////////////////////////////////
    /// CustomCommandList overrides
    void _InitializeCommands();
};

}
