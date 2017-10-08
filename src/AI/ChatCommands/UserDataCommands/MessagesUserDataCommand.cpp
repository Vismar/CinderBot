/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MessagesUserDataCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Database;

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
    QString msg = QString::number(UserDataDBHelper::GetUserParameter(UserDataParameter::Messages, message.GetUserID()).toInt());
    (*answer.GetAnswers().begin()).replace("MSG_COUNT", msg);
}

///////////////////////////////////////////////////////////////////////////

void MessagesUserDataCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
