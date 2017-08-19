/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include "AI/ChatCommands/CommandList.hpp"
#include "AI/ChatCommands/CustomCommands/CustomChatCommand.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

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
 * \brief Reads and store all custom commands from database.
 * 
 * Handles creating and storing commands that can be executed by users vis chat or whispers.
 * Also updates itself if new command was added or already existing one was deleted.
 */
class CustomCommandList : public CommandList
{
    Q_OBJECT
public:
    CustomCommandList();

protected:
    /*////////////////////////////*/
    /* CommandList override       */
    /*////////////////////////////*/
    /*!
     * \brief Initializes custom command list.
     *
     * Intializes needed parameters and data. Connects events from CustomCommandDBHelper.
     */
    void Initialize() override;
    /*!
     * \brief Catch changes in configuration manager.
     * \param cfgParam - configuration parameter.
     *
     * Catches change of configuration parameter to turn on and off command list.
     */
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam) override;

    /*!
     * \brief Returns created custom command.
     * \return Pointer to created command.
     */
    virtual CustomChatCommand *_CreateCommand() const;

    /*!
     * \brief Initializes command list.
     * 
     * Gets all custom commands and adds it to command list.
     */
    virtual void _InitializeCommands();

    /*! Type of command list */
    Utils::Database::CmdType _commandType;
    /*! Command module to which this command list is related */
    Utils::Configuration::CfgParam _cmdModule;

private slots:
    /*!
     * \brief Adds new command to list.
     * 
     * Check if cmdType is the same as _commandType and adds new command to list.
     */
    void _AddCommand(Utils::Database::CmdType cmdType, const QString &cmdName, int id);
    /*!
     * \brief Deletes command from list.
     *
     * Check if cmdType is the same as _commandType and removes command from list.
     */
    void _DeleteCommand(Utils::Database::CmdType cmdType, const QString &cmdName, int id);

private:
    /*!
     * \brief Creates and adds command to command list.
     * 
     * Creates, initializes and adds created command to _commands.
     */
    void _AddCommandToList(const QString &cmdName);
};

}
}
