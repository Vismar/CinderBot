/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ListCovenantCommand.hpp"
#include <Utils/DatabaseManager.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

ListCovenantCommand::ListCovenantCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void ListCovenantCommand::Initialize()
{
    _name = "!cov_list";
    _answers.push_back("Covenants: COV_LIST");
}

///////////////////////////////////////////////////////////////////////////

void ListCovenantCommand::_GetAnswer(const ChatMessage& message, QString& answer)
{
    Q_UNUSED(message);

    QString covList;
    // Get covenant list
    DB_QUERY_PTR query = DB_SELECT("Covenants", "Name");
    if (query->exec())
    {
        answer = _answers.first();
        while (query->next())
        {
            covList.append(query->value("Name").toString() + ", ");
        }
        if (covList.size() > 0)
        {
            covList = covList.left(covList.size() - 2);
        }
    }
    answer.replace("COV_LIST", covList);
}

///////////////////////////////////////////////////////////////////////////

void ListCovenantCommand::_GetRandomAnswer(const ChatMessage& message, QString& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
