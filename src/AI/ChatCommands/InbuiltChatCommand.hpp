/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/BaseChatCommand.hpp"
#include <QVector>

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{

/*!
 * class InbuiltChatCommand
 * Base class for inbuilt commands, which includes array
 * that can be used as a store for answers that should be translated
 */
class InbuiltChatCommand : public BaseChatCommand
{
public:
    InbuiltChatCommand();
    virtual ~InbuiltChatCommand();

protected:
    /*! List of answers */
    QVector<QString> _answers;
};

}
