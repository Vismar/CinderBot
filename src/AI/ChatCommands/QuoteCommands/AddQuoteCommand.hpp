/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "BaseQuoteCommand.hpp"

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{
/*!
 * \brief All quote related commands.
 */
namespace QuoteCmd
{

/*!
 * \brief Chat command that provide functionality to add quote.
 */
class AddQuoteCommand : public BaseQuoteCommand
{
public:
    AddQuoteCommand();

protected:
    /*///////////////////////////////*/
    /*   BaseChatCommand overrides   */
    /*///////////////////////////////*/
    /*!
     * \brief Initializes command.
     * 
     * Sets name, moderator flag and adds predefined answers.
     */
    void Initialize() override;
    /*!
     * \brief Gets specified answer.
     * \param message - chat message.
     * \param answer - reference to variable which will store answer.
     *
     * Check if user is moderator and helps to add quote.
     */
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) override;
    /*!
     * \brief Empty function.
     */
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) override;
};

}
}
