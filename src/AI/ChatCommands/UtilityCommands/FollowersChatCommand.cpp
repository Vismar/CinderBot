/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "FollowersChatCommand.hpp"
#include "Twitch/KrakenClient.hpp"

using namespace Command::UtilityCmd;
using namespace Twitch;

///////////////////////////////////////////////////////////////////////////

FollowersChatCommand::FollowersChatCommand()
{
    _name = "!followers";
    _answers.push_back("Followers: ");
}

///////////////////////////////////////////////////////////////////////////

void FollowersChatCommand::_GetAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    answer.AddAnswer(_answers.at(0) + KrakenClient::Instance().GetParameter(KrakenParameter::ChannelFollowers).toString());
}

///////////////////////////////////////////////////////////////////////////

void FollowersChatCommand::_GetRandomAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
