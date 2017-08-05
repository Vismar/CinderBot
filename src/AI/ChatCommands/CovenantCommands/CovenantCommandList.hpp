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
namespace CovenantCmd
{

/*!
 * class CovenantCommandList
 * Store all covenant commands
 */
class CovenantCommandList : public CommandList
{
public:
    /*! Constructor */
    CovenantCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void Initialize();
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam);
};

}
}
