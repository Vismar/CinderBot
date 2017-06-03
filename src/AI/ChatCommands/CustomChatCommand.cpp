/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomChatCommand.hpp"
#include <Utils/DatabaseManager.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

CustomChatCommand::CustomChatCommand() {}

///////////////////////////////////////////////////////////////////////////

CustomChatCommand::~CustomChatCommand() {}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::Initialize()
{
    // Get data about command
    DB_QUERY_PTR query = DB_SELECT("CustomCommands", "*", QString("Name='%1'").arg(_name));
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

void CustomChatCommand::_GetAnswer(const ChatMessage& message, QString& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::_GetRandomAnswer(const ChatMessage& message, QString& answer)
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
        DB_QUERY_PTR query = DB_SELECT("CustomCommandAnswers", "*",
                                       QString("Answer IN (SELECT Answer FROM CustomCommandAnswers "
                                       "WHERE Name='%1' ORDER BY RANDOM() LIMIT 1)").arg(_name));
        if (query != NULL)
        {
            if (query->first())
            {
                answer = query->value("Answer").toString();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
