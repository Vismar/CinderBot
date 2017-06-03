/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "BaseQuoteCommand.hpp"

namespace Command
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
    void _GetAnswer(const ChatMessage& message, QString& answer);
    void _GetRandomAnswer(const ChatMessage& message, QString& answer);

private:
    /*!
     * Update quote numbers
     * \param(IN) quoteNumber - number of deleted quote
     */
    void _RefreshQuoteNumbers(int quoteNumber);
};

}
