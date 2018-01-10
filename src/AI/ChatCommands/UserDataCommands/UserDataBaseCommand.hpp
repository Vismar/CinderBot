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
 * Chat commands related to user data.
 */
namespace UserDataCmd
{

/*!
 * \brief Base class for all user data commands.
 *
 * Contains connection to configuration manager to update chat and whisper flags.
 */
class UserDataBaseCommand : public InbuiltChatCommand
{
public:
    UserDataBaseCommand();
    virtual ~UserDataBaseCommand();

private slots:
    /*!
     * \brief _UpdateChatAndWhisperFlags
     * \param cfgParam
     */
    void _UpdateChatAndWhisperFlags(Utils::Configuration::CfgParam cfgParam);
};

}
}
