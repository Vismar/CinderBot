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
namespace UtilityCmd
{

class UtilityCommandList : public CommandList
{
public:
    /*!
     * \brief Default constructor.
     */
    UtilityCommandList();

private:
    /*///////////////////////////////*/
    /* CovenantBaseCommand overrides */
    /*///////////////////////////////*/
    /*!
     * \brief Empty function.
     */
    void Initialize() override { }
    /*!
     * \brief Empty function.
     */
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam) override;
};

}
}