/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "QuotesWidget.hpp"
#include "SingleQuoteWidget.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Ui::Quote;

///////////////////////////////////////////////////////////////////////////

QuotesWidget::QuotesWidget(QWidget *parent) : PageListWidget(parent) {}

///////////////////////////////////////////////////////////////////////////

QuotesWidget::~QuotesWidget() {}

///////////////////////////////////////////////////////////////////////////

void QuotesWidget::_CreateAndAddWidget()
{
    SingleQuoteWidget *widget = new SingleQuoteWidget();
    _AddWidget(widget);
}

///////////////////////////////////////////////////////////////////////////

void QuotesWidget::_UpdateEntry(QWidget *entry, int id)
{
    SingleQuoteWidget *entryWidget = static_cast<SingleQuoteWidget*>(entry);
    if (entryWidget != nullptr)
    {
        DB_QUERY_PTR query = DB_SELECT("Quotes", "Quote, Number", QString("Id=%1").arg(id));
        if (query != nullptr)
        {
            if (query->first())
            {
                entryWidget->SetQuoteText(query->value("Quote").toString());
                entryWidget->SetQuoteNumber(query->value("Number").toString());
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
