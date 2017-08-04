/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "TimeInChatCommand.hpp"
#include "Utils/UserData/UserData.hpp"

using namespace Command::UserDataCmd;

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
    QTime timeInChat(0, 0);
    int time = UD_GET_PARAM(message.GetRealName(), UDP_TimeInChat).toInt();
    timeInChat = timeInChat.addSecs(time * 60);
    (*answer.GetAnswers().begin()).replace("MSG_TIME_IN_CHAT", timeInChat.toString("hh:mm"));
}

///////////////////////////////////////////////////////////////////////////

void TimeInChatCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
