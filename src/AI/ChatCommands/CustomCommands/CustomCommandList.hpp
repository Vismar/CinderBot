/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CommandList.hpp"

namespace Command
{

/*!
 * Class CustomCommandList
 * Read and store all custom commands from database
 */
class CustomCommandList : public CommandList
{
public:
    /*! Constructor */
    CustomCommandList();
    /*! Destructor */
    virtual ~CustomCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void _Initialize();

    /*!
     * Initialize command list with proper commands from database
     */
    virtual void _InitializeCommands();

    /*! Name of table which should be created to store commands */
    QString _commandTableName;
    /*! Name of table which should be created to store answers for commands */
    QString _commandAnswersTableName;
};

}
