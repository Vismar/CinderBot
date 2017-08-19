/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CustomCommands/CustomCommandList.hpp"

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
 * \brief Reads and stores all custom commands for covenants.
 * 
 * Inherits the CustomCommandList and changes _commandType and _cmdModule to initialize command from covenant commands table.
 */
class CustomCovCommandList : public CustomCommandList
{
public:
    CustomCovCommandList();

protected:
    /*////////////////////////////*/
    /* CustomCommandList override */
    /*////////////////////////////*/
    /*!
     * \brief Returns created custom cov command.
     * \return Pointer to created command.
     */
    Command::CustomChatCmd::CustomChatCommand *_CreateCommand() const override;
};

}
}
