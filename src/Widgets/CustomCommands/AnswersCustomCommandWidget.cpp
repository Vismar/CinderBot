/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AnswersCustomCommandWidget.hpp"
#include "Widgets/CustomCommands/EditSaveAnswerCustomCommandWidget.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

AnswersCustomCommandWidget::AnswersCustomCommandWidget(QWidget *parent) : PageListWidget(parent) {}

///////////////////////////////////////////////////////////////////////////

AnswersCustomCommandWidget::~AnswersCustomCommandWidget() {}

///////////////////////////////////////////////////////////////////////////

void AnswersCustomCommandWidget::_CreateAndAddWidget()
{
    EditSaveAnswerCustomCommandWidget *widget = new EditSaveAnswerCustomCommandWidget();
    _AddWidget(widget);
}

///////////////////////////////////////////////////////////////////////////

void AnswersCustomCommandWidget::_UpdateEntry(QWidget *entry, int id)
{
    EditSaveAnswerCustomCommandWidget *entryWidget = static_cast<EditSaveAnswerCustomCommandWidget*>(entry);
    if (entryWidget != nullptr)
    {
        DB_QUERY_PTR query = DB_SELECT("CustomCommandAnswers", "Answer", QString("Id=%1").arg(id));
        if (query != nullptr)
        {
            if (query->first())
            {
                entryWidget->SetId(id);
                entryWidget->SetText(query->value("Answer").toString());
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
