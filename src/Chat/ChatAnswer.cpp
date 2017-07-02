/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ChatAnswer.hpp"

///////////////////////////////////////////////////////////////////////////

ChatAnswer::ChatAnswer()
: _type(Twitch_Chat)
{
}

///////////////////////////////////////////////////////////////////////////

ChatAnswer::~ChatAnswer() { }

///////////////////////////////////////////////////////////////////////////

ChatAnswerType ChatAnswer::GetType() const
{
    return _type;
}

///////////////////////////////////////////////////////////////////////////

void ChatAnswer::SetType(ChatAnswerType type)
{
    _type = type;
}

///////////////////////////////////////////////////////////////////////////

const QString& ChatAnswer::GetRealName() const
{
    return _realName;
}

///////////////////////////////////////////////////////////////////////////

void ChatAnswer::SetRealName(const QString &realName)
{
    _realName = realName;
}

///////////////////////////////////////////////////////////////////////////

void ChatAnswer::AddAnswer(const QString &answer)
{
    _answers.append(answer);
}

///////////////////////////////////////////////////////////////////////////

QStringList& ChatAnswer::GetAnswers()
{
    return _answers;
}

///////////////////////////////////////////////////////////////////////////
