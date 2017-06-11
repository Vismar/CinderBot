/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomChatCommand.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CustomChatCommand::CustomChatCommand()
{
    _commandTableName = "CustomCommands";
    _commandAnswersTableName = "CustomCommandAnswers";
}

///////////////////////////////////////////////////////////////////////////

CustomChatCommand::~CustomChatCommand() {}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::Initialize()
{
    // Get data about command
    DB_QUERY_PTR query = DB_SELECT(_commandTableName, "*", QString("Name='%1'").arg(_name));
    if (query != NULL)
    {
        if (query->first())
        {
            // Set data to variables
            _cooldown = QTime::fromString(query->value("Cooldown").toString(), "h:m:s:z");
            _moderatorOnly = query->value("ModeratorOnly").toBool();
            _price = query->value("Price").toInt();
            _covenant = query->value("Covenant").toString();
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::InitializeByName(const QString& commandName)
{
    _Clear();
    _name = commandName;
    _isRandom = true;
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::_GetAnswer(const ChatMessage& message, QStringList& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::_GetRandomAnswer(const ChatMessage& message, QStringList& answer)
{
    if (_CheckModerationFlag(message.IsModerator()) &&
        _CheckCooldown() &&
        _CheckCurrency(message.GetRealName()) &&
        _CheckCovenant(message.GetRealName()))
    {
        // Update user currency value
        _TakeDefaultPriceFromUser(message.GetRealName());
        // Save time of exection
        _lastTimeUsed = QDateTime::currentDateTime();
        // Get random answer
        DB_QUERY_PTR query = DB_SELECT(_commandAnswersTableName, "*",
                                       QString("Answer IN (SELECT Answer FROM %1 "
                                       "WHERE Name='%2' "
                                       "ORDER BY RANDOM() LIMIT 1)").arg(_commandAnswersTableName).arg(_name));
        if (query != NULL)
        {
            if (query->first())
            {
                QString longAnswer = query->value("Answer").toString();
                // If answer is less than 500 symbols, then everything ok
                if (longAnswer.length() <= 500)
                {
                    answer.append(longAnswer);
                }
                // If answer is longer than 500 symbols, so it should be sliced
                else
                {
                    // Slice an answer to separate messages
                    // until every part of an answer will not be less than 500 symbols
                    while (longAnswer.length() > 0)
                    {
                        QString part = longAnswer.left(500);
                        // Try to find last space in this part
                        int lastSpace = part.lastIndexOf(' ');
                        // If in this part no space were found, just use 500 symbols there
                        if (lastSpace != -1)
                        {
                            answer.append(longAnswer.left(lastSpace+1));
                            longAnswer.remove(0, lastSpace+1);
                        }
                        // If last space were found, cut the message until this index
                        else
                        {
                            answer.append(part);
                            longAnswer.remove(0, 500);
                        }
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
