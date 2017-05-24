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
 * Class EditQuoteCommand
 * Edit quote command
 */
class EditQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    EditQuoteCommand();

    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
};

}
