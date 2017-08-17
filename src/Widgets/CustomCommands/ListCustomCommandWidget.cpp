/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ListCustomCommandWidget.hpp"
#include "Widgets/CustomCommands/EntryCustomCommandWidget.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

using namespace Ui::CustomCommand;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

ListCustomCommandWidget::ListCustomCommandWidget(QWidget *parent) : PageListWidget(parent) {}

///////////////////////////////////////////////////////////////////////////

ListCustomCommandWidget::~ListCustomCommandWidget() {}

///////////////////////////////////////////////////////////////////////////

void ListCustomCommandWidget::_CreateAndAddWidget()
{
    EntryCustomCommandWidget *widget = new EntryCustomCommandWidget();
    _AddWidget(widget);
}

///////////////////////////////////////////////////////////////////////////

void ListCustomCommandWidget::_UpdateEntry(QWidget *entry, int id)
{
    EntryCustomCommandWidget *entryWidget = static_cast<EntryCustomCommandWidget*>(entry);
    if (entryWidget != nullptr)
    {
        QString cmdName = CustomCommandDBHelper::Instance().GetParameter(CmdType::StreamerCmd, id, CustomCmdParameter::Name);
        entryWidget->SetCmdName(cmdName);
    }
}

///////////////////////////////////////////////////////////////////////////
