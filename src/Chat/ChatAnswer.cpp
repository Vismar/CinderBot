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

void ChatAnswer::AddAnswer(const QString &answer)
{
    _answers.append(answer);
}

///////////////////////////////////////////////////////////////////////////

const QStringList& ChatAnswer::GetAnswers() const
{
    return _answers;
}

///////////////////////////////////////////////////////////////////////////
