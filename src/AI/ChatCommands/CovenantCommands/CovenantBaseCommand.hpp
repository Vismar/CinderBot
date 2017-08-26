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

/*!
 * \brief Base class for all covenant commands.
 * 
 * Base class for all covenant commands that handles changes of configuration parameters to
 * update work in whisper and work in chat flags.
 */
class CovenantBaseCommand : public InbuiltChatCommand
{
public:
    CovenantBaseCommand();
    virtual ~CovenantBaseCommand();

private slots:
    /*!
     * \brief Updates work in whisper and work in chat flags when they are changed.
     * \param cfgParam - Configuration parameter that was changed.
     */
    void _UpdateChatAndWhisperFlags(Utils::Configuration::CfgParam cfgParam);
};

}
}
