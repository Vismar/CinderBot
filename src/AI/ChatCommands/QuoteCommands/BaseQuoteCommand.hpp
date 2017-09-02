/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/InbuiltChatCommand.hpp"
#include <QRegularExpression>

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
 * \brief Base class for every quote command.
 * 
 * This is the base for every quote command. The class has function to find number after a command.
 */
class BaseQuoteCommand : public InbuiltChatCommand
{
public:
    BaseQuoteCommand();

protected:
    /*!
     * \brief Tries to get number right after command.
     * \param message - message in which number should be found.
     * \param value - number that was found.
     * \return True, if number was found. Otherwise - false.
     * 
     * Tries to find number that was specified after command. If value was found, returns true and sets value to parameter "value".
     */
    bool _GetNumberAfterCommand(const QString &message, QString &value) const;

private:
    /*! Refular expression to find number after command */
    QRegularExpression _regExpNumber;
};

}
}
