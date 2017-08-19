/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EditSaveAnswerCustomCommandWidget.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

using namespace Ui::CustomCommand;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

EditSaveAnswerCustomCommandWidget::EditSaveAnswerCustomCommandWidget(QWidget *parent) : EditSaveWidget(parent) { }

///////////////////////////////////////////////////////////////////////////

EditSaveAnswerCustomCommandWidget::~EditSaveAnswerCustomCommandWidget() { }

///////////////////////////////////////////////////////////////////////////

void EditSaveAnswerCustomCommandWidget::_SaveEntry()
{
    CustomCommandDBHelper::Instance().EditAnswer(CmdType::StreamerCmd, _id, _text->toPlainText());
}

///////////////////////////////////////////////////////////////////////////

void EditSaveAnswerCustomCommandWidget::_DeleteEntry()
{
    CustomCommandDBHelper::Instance().DeleteAnswer(CmdType::StreamerCmd, _id);
}

///////////////////////////////////////////////////////////////////////////
