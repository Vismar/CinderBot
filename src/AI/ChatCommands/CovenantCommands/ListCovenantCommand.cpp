/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ListCovenantCommand.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

ListCovenantCommand::ListCovenantCommand()
{
    // 1 min cooldown
    _cooldown.setHMS(0, 1, 0, 0);
    _name = "!cov_list";
    _answers.push_back("Covenants: COV_LIST");
}

///////////////////////////////////////////////////////////////////////////

void ListCovenantCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);

    answer.AddAnswer(_answers.first());
    (*answer.GetAnswers().begin()).replace("COV_LIST", CovenantDBHelper::GetCovenants().join(", "));
}

///////////////////////////////////////////////////////////////////////////

void ListCovenantCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
