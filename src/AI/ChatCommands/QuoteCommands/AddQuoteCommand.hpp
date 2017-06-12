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
 * Class AddQuoteCommand
 * Add quote command
 */
class AddQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    AddQuoteCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage &message, QStringList &answer);
    void _GetRandomAnswer(const ChatMessage &message, QStringList &answer);
};

}
