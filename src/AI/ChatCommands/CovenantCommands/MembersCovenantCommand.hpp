/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CovenantCommands/CovenantBaseCommand.hpp"
#include <QRegularExpression>

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
 * \brief Display members of certain covenant.
 *
 * Command to display members of specified covenant or covenant in which user is if no covenant was specified.
 */
class MembersCovenantCommand : public CovenantBaseCommand
{
public:
    MembersCovenantCommand();

protected:
    /*///////////////////////////////*/
    /* BaseChatCommand overrides     */
    /*///////////////////////////////*/
    /*!
     * \brief Empty function.
     */
    void Initialize() override {}
    /*!
     * \brief Gets specified answer.
     * \param message - chat message.
     * \param answer - reference to variable which will store answer.
     */
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) override;
    /*!
     * \brief Empty function.
     */
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) override;

private:
    /*! Regular expression to get covenant name */
    QRegularExpression _covNameExpression;
};

}
}
