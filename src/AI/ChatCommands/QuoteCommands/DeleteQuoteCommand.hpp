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
 * Class DeleteQuoteCommand
 * Delete quote command
 */
class DeleteQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    DeleteQuoteCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer);
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer);

private:
    /*!
     * Update quote numbers
     * \param(IN) quoteNumber - number of deleted quote
     */
    void _RefreshQuoteNumbers(int quoteNumber);
};

}
}
