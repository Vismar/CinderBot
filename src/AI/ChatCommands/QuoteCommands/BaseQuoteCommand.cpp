/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "BaseQuoteCommand.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

BaseQuoteCommand::BaseQuoteCommand() {}

///////////////////////////////////////////////////////////////////////////

bool BaseQuoteCommand::_GetNumberAfterCommand(const QString &command, const QString &message, QString &value)
{
    bool isDigit(false);
    // Get start of a command
    int id = message.indexOf(command);
    if (id != -1)
    {
        // Check size
        if ((id + command.size()) < message.size())
        {
            isDigit = true;
            // Get string that should contain number
            int startPosOfSearch = id + command.size() + 1;
            int indexOfSpace = message.indexOf(" ", startPosOfSearch);
            if (indexOfSpace != -1)
            {
                value = message.mid(startPosOfSearch, indexOfSpace - startPosOfSearch);
            }
            else
            {
                value = message.right(message.size() - startPosOfSearch);
            }
            for (int i = 0; i < value.size(); ++i)
            {
                if (!value.at(i).isDigit())
                {
                    isDigit = false;
                }
            }
        }
    }

    return isDigit;
}

///////////////////////////////////////////////////////////////////////////
