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
 * \brief Handles joining users to covenants.
 */
class JoinCovenantCommand : public CovenantBaseCommand
{
public:
    JoinCovenantCommand();

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
