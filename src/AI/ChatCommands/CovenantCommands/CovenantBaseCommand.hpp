/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/InbuiltChatCommand.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

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

class CovenantBaseCommand : public InbuiltChatCommand
{
public:
    CovenantBaseCommand();
    virtual ~CovenantBaseCommand();

private slots:
    /*!
     * \brief _UpdateChatAndWhisperFlags
     * \param cfgParam
     */
    void _UpdateChatAndWhisperFlags(Utils::Configuration::CfgParam cfgParam);
};

}
}
