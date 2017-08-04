/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/InbuiltChatCommand.hpp"

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{
namespace QuoteCmd
{

/*!
 * Class BaseCovenantCommand
 * Base class for every quote command
 */
class BaseQuoteCommand : public InbuiltChatCommand
{
public:
    BaseQuoteCommand();

protected:
    /*!
     * Try to get number right after command.
     * \param(IN) command - command itself
     * \param(IN) message - message in which number should be found
     * \param(OUT) value - number that was found
     * \return true, if number was found
     */
    bool _GetNumberAfterCommand(const QString &command, const QString &message, QString &value);
};

}
}
