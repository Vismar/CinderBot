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
 * \brief Covenant command to add/edit and display command answers.
 * 
 * This command can take 3 parameters: name, id and answer.
 * Use of parameter looks like this: name=value; id=number; answer=string;.
 * To see all answer ids specify command name.
 * To see certain answer specify command name and id of the answer.
 * To edit an answer, specify command name, id and answer.
 * To add an answer, specify command name and answer.
 */
class AdminCovCmdAnswerCommand : public CovenantBaseCommand
{
public:
    AdminCovCmdAnswerCommand();

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
     * Check if user is covenant leader and helps to add/edit covenant command answers.
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
    /*! Regular expression to find variable "answer". */
    QRegularExpression _regExpAnswer;
};

}
}
