/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MessagesUserDataCommand.hpp"
#include "Utils/UserData/UserData.hpp"

using namespace Command::UserDataCmd;

///////////////////////////////////////////////////////////////////////////

MessagesUserDataCommand::MessagesUserDataCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void MessagesUserDataCommand::Initialize()
{
    _name = "!msg";
    _answers.push_back("You had wrote MSG_COUNT messages, @!");
}

///////////////////////////////////////////////////////////////////////////

void MessagesUserDataCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    answer.AddAnswer(_answers.first());
    (*answer.GetAnswers().begin()).replace("MSG_COUNT", UD_GET_PARAM(message.GetRealName(), UDP_Messages));
}

///////////////////////////////////////////////////////////////////////////

void MessagesUserDataCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
