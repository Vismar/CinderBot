/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CustomCommands/CustomChatCommand.hpp"

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{
/*!
 * \brief Contains all custom chat commands.
 */
namespace CustomChatCmd
{

/*!
 * \brief Small class for custom covenant commands.
 * 
 * This class almost the same as CustomChatCommand. 
 * Only differences - different _cmdType and connection with different ype of events from CustomCommandDBHelper.
 */
class CustomCovChatCommand : public CustomChatCommand
{
public:
    CustomCovChatCommand();

protected:
    /*////////////////////////////*/
    /* CustomChatCommand overrides*/
    /*////////////////////////////*/
    /*!
     * \brief Connects events for custom cov commands.
     */
    void _ConnectEvents() override;
};

}
}
