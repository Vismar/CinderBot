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
 * \brief All quote related commands.
 */
namespace QuoteCmd
{

/*!
 * Class QuoteCommandList
 * List of quote commands
 */
class QuoteCommandList : public CommandList
{
public:
    QuoteCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void Initialize() override;
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam) override;
};

}
}
