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
 * Class MainQuoteCommand
 * Main quote command
 */
class MainQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    MainQuoteCommand();

    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
};

}
