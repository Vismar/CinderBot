/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AnswersCustomCommandWidget.hpp"
#include "Widgets/CustomCommands/EditSaveAnswerCustomCommandWidget.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

using namespace Ui::CustomCommand;
using namespace Utils::Database;

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
        QString answer = CustomCommandDBHelper::Instance().GetAnswer(CmdType::StreamerCmd, id);

        entryWidget->SetId(id);
        entryWidget->SetText(answer);
    }
}

///////////////////////////////////////////////////////////////////////////
