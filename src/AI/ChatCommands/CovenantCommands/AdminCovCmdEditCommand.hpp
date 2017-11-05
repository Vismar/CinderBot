/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CovenantCommands/CovenantBaseCommand.hpp"

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
 * \brief Covenant command to update/create commands.
 * 
 * Displays info about command if not arguments were specified.
 * This command can take 3 parameters: name, cooldown, and price.
 * Use of parameter looks like this: name=value; cooldown=h:m:s.; price=100;.
 * If you want to edit your command specify its name and specify 
 * the parameter that you want to change (you can specify both of them in 1 message).
 * If you will do the same, but specified command does not exist 
 * then new command will be created if your covenant has free command slots.
 */
class AdminCovCmdEditCommand : public CovenantBaseCommand
{
public:
    AdminCovCmdEditCommand();

    /*!
     * \brief Empty function.
     */
    void Initialize() override {}

protected:
    /*///////////////////////////////*/
    /* CovenantBaseCommand overrides */
    /*///////////////////////////////*/
    /*!
     * \brief Gets specified answer.
     * \param message - chat message.
     * \param answer - reference to variable which will store answer.
     *
     * Check if user is covenant leader and helps to create and edit covenant commands.
     */
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) override;
    /*!
     * \brief Empty function.
     */
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) override;

private:
    /*!
     * \brief Handle the updating/creating command with three params: name, cooldown and price.
     * \param cmdName - Name of command that will be updated/created.
     * \param cooldown - Cooldown that will be setted to command.
     * \param price - Price that will be setted to command.
     * \param covenant - Covenant for which command should be updated/created.
     * \param answer - Reference to answer that will be used.
     * \param numberOfCommands - Number of commands that was already created for covenant.
     * \param cmdSlots - Number of command slots that are available.
     */
    void _HandleThreeParams(const QString &cmdName, const QString &cooldown, const QString &price, const QString &covenant, 
                            ChatAnswer &answer, int numberOfCommands, int cmdSlots) const;
    /*!
     * \brief Handle the updating/creating command with two params: name and cooldown.
     * \param cmdName - Name of command that will be updated/created.
     * \param cooldown - Cooldown that will be setted to command.
     * \param covenant - Covenant for which command should be updated/created.
     * \param answer - Reference to answer that will be used.
     * \param numberOfCommands - Number of commands that was already created for covenant.
     * \param cmdSlots - Number of command slots that are available.
     */
    void _HandleCooldown(const QString &cmdName, const QString &cooldown, const QString &covenant,
                         ChatAnswer &answer, int numberOfCommands, int cmdSlots) const;
    /*!
     * \brief Handle the updating/creating command with two params: name and price.
     * \param cmdName - Name of command that will be updated/created.
     * \param price - Price that will be setted to command.
     * \param covenant - Covenant for which command should be updated/created.
     * \param answer - Reference to answer that will be used.
     * \param numberOfCommands - Number of commands that was already created for covenant.
     * \param cmdSlots - Number of command slots that are available.
     */
    void _HandlePrice(const QString &cmdName, const QString &price, const QString &covenant,
                      ChatAnswer &answer, int numberOfCommands, int cmdSlots) const;

    /*!
     * \brief Updates command.
     * \param cmdName - Name of command that will be updated.
     * \param cooldown - Cooldown that will be setted to updated command.
     * \param price - Price that will be setted to updated command.
     */
    static void _UpdateCommand(const QString &cmdName, const QString &cooldown = "", const QString &price = "");
    /*!
     * \brief Creates command.
     * \param cmdName - Name of command that will be created.
     * \param covenant - Covenant for which command should be created.
     * \param cooldown - Cooldown that will be setted to created command.
     * \param price - Price that will be setted to created command.
     */
    bool _CreateCommand(const QString &cmdName, const QString &covenant, const QString &cooldown = "", const QString &price = "") const;

    /*! Regular expression to find variable "name". */
    QRegularExpression _regExpName;
    /*! Regular expression to find variable "cooldown". */
    QRegularExpression _regExpCooldown;
    /*! Regular expression to find variable "price". */
    QRegularExpression _regExpPrice;
};
        
}
}