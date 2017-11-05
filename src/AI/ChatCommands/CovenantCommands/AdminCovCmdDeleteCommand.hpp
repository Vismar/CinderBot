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
 * \brief Covenant command to delete commands/answers.
 * 
 * Displays info about command if not arguments were specified.
 * This command can take 2 parameters: name and id.
 * Use of parameter looks like this: name=value; id=number;.
 * To delete command you need to specify its name and nothing more.
 * To delete certain answer, then specify command name and id of answer.
 */
class AdminCovCmdDeleteCommand : public CovenantBaseCommand
{
public:
    AdminCovCmdDeleteCommand();

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
     * Check if user is covenant leader and helps to delete covenant commands and their answers.
     */
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) override;
    /*!
     * \brief Empty function.
     */
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) override;

private:
    /*! Regular expression to find variable "name". */
    QRegularExpression _regExpName;
    /*! Regular expression to find variable "id". */
    QRegularExpression _regExpId;
};
    
}
}