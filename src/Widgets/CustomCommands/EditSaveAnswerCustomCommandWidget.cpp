/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EditSaveAnswerCustomCommandWidget.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Ui::CustomCommand;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

EditSaveAnswerCustomCommandWidget::EditSaveAnswerCustomCommandWidget(QWidget *parent) : EditSaveWidget(parent) { }

///////////////////////////////////////////////////////////////////////////

EditSaveAnswerCustomCommandWidget::~EditSaveAnswerCustomCommandWidget() { }

///////////////////////////////////////////////////////////////////////////

void EditSaveAnswerCustomCommandWidget::_SaveEntry()
{
    std::shared_ptr<QSqlQuery> query = DB_SELECT("CustomCommandAnswers", "Answer", QString("Id = %1").arg(_id));
    if (query != NULL)
    {
        query->first();
        // Save new text if it was changed
        if (query->value("Answer").toString() != _text->toPlainText())
        {
            DB_UPDATE("CustomCommandAnswers", QString("Answer='%1'").arg(_text->toPlainText()),
                                              QString("Id=%1").arg(_id));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void EditSaveAnswerCustomCommandWidget::_DeleteEntry()
{
    DB_DELETE("CustomCommandAnswers", QString("Id=%1").arg(_id));
}

///////////////////////////////////////////////////////////////////////////
