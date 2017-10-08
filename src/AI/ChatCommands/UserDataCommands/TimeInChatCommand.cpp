/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "TimeInChatCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

TimeInChatCommand::TimeInChatCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void TimeInChatCommand::Initialize()
{
    _name = "!time";
    _answers.push_back("Time spent in chat - MSG_TIME_IN_CHAT, @!");
}

///////////////////////////////////////////////////////////////////////////

void TimeInChatCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    answer.AddAnswer(_answers.first());
    // Time in chat
    int minutes = UserDataDBHelper::GetUserParameter(UserDataParameter::TimeInChat, message.GetUserID()).toInt();
    int hours = minutes / 60;
    minutes %= 60;
    (*answer.GetAnswers().begin()).replace("MSG_TIME_IN_CHAT", QString("%1h%2m").arg(hours).arg(minutes));
}

///////////////////////////////////////////////////////////////////////////

void TimeInChatCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
