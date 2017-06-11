/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/QuoteCommands/BaseQuoteCommand.hpp"

namespace Command
{

/*!
 * Class MainQuoteCommand
 * Main quote command
 */
class MainQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    MainQuoteCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage& message, QStringList& answer);
    void _GetRandomAnswer(const ChatMessage& message, QStringList& answer);
};

}
