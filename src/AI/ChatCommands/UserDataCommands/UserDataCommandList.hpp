/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CommandList.hpp"

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{
/*!
 * Chat commands related to user data.
 */
namespace UserDataCmd
{

/*!
 * Class UserDataCommandList
 * Store all UD commands
 */
class UserDataCommandList : public CommandList
{
public:
    UserDataCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void Initialize();
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam);
};

}
}
