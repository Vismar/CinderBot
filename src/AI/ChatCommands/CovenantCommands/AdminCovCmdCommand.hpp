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
 * \brief Covenant command to display info about commands.
 * 
 * Covenant command that will display all commands if no param specified.
 * If user specified param "name" then command will return info about specified command.
 */
class AdminCovCmdCommand : public CovenantBaseCommand
{
public:
    AdminCovCmdCommand();

    /*!
     * \brief Initialize command parameters.
     */
    void Initialize() override;

protected:
    /*///////////////////////////////*/
    /* CovenantBaseCommand overrides */
    /*///////////////////////////////*/
    /*!
     * \brief Gets specified answer.
     * \param message - chat message.
     * \param answer - reference to variable which will store answer.
     * 
     * Check if user is covenant leader and displays information about covenant command.
     */
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) override;
    /*!
     * \brief Empty function.
     */
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) override;

private:
    /*! Regular expression to find variable "name". */
    QRegularExpression _regExpName;
};

}
}