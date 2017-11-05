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
 * \brief All covenant related commands.
 */
namespace CovenantCmd
{

/*!
 * \brief Stores all covenant commands.
 */
class CovenantCommandList : public CommandList
{
public:
    CovenantCommandList();

protected:
    /*///////////////////////////////*/
    /* CommandList overrides         */
    /*///////////////////////////////*/
    /*!
     * \brief Calls OnCfgParamChanged to update flag.
     */
    void Initialize() override;
    /*!
     * \brief Check if CfgParam::CovenantCmdModule is turned 'on' or 'off'.
     */
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam) override;
};

}
}
