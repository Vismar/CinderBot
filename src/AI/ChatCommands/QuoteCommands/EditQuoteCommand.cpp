/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EditQuoteCommand.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

EditQuoteCommand::EditQuoteCommand()
{
    _Clear();
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void EditQuoteCommand::Initialize()
{
    _name = "!quote_edit";
    _moderatorOnly = true;
    _answers.push_back("Quote #QUOTE_NUMBER was edited!");
}

///////////////////////////////////////////////////////////////////////////

void EditQuoteCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    if (_CheckModerationFlag(message.IsModerator()))
    {
        QString val;
        // Try to found number after command
        if (_GetNumberAfterCommand(_name, message.GetMessage(), val))
        {
            // Check borders
            int number = val.toInt();
            std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(Number)");
            if (numberQuery != NULL)
            {
                numberQuery->first();
                int maxValue = numberQuery->value(0).toInt() + 1;
                if ((number > 0) && (number <= maxValue))
                {
                    // Remove part with number from message
                    const QString &refMsg = message.GetMessage();
                    QString msg = refMsg.right(refMsg.size()-refMsg.indexOf(_name)-_name.size()-val.size()-2);
                    // Update quote
                    if(DB_UPDATE("Quotes", QString("Quote = '%1'").arg(msg), QString("Number = %1").arg(number)))
                    {
                        answer.AddAnswer(_answers.at(0));
                        (*answer.GetAnswers().begin()).replace("QUOTE_NUMBER", QString::number(number));
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void EditQuoteCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
