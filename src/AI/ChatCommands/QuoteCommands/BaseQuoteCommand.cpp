/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "BaseQuoteCommand.hpp"
#include <QRegularExpressionMatch>

using namespace Command::QuoteCmd;

///////////////////////////////////////////////////////////////////////////

BaseQuoteCommand::BaseQuoteCommand()
{
    _regExpNumber.setPattern(" (?<number>[0-9]+)");
}

///////////////////////////////////////////////////////////////////////////

bool BaseQuoteCommand::_GetNumberAfterCommand(const QString &message, QString &value) const
{
    bool found(false);
    QRegularExpressionMatch numberMatch = _regExpNumber.match(message);

    if (numberMatch.hasMatch())
    {
        value = numberMatch.captured("number");
        found = true;
    }

    return found;
}

///////////////////////////////////////////////////////////////////////////
