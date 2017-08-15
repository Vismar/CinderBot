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
namespace CustomChatCmd
{

class CustomCovChatCommand : public CustomChatCommand
{
public:
    CustomCovChatCommand();
};

}
}
