/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "QuotesWidget.hpp"
#include "SingleQuoteWidget.hpp"
#include <Utils/DatabaseManager.hpp>
#include <QScrollBar>

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

QuotesWidget::QuotesWidget(QWidget* parent) : QScrollArea(parent)
{
    // Initialize layout
    setWidgetResizable(true);
    _container = new QWidget();
    this->setWidget(_container);
    _layout = new QVBoxLayout(_container);
    _layout->setMargin(10);
    _layout->setAlignment(Qt::AlignTop);

    // Create needed widgets and get all quotes
    UpdateQuotes("Quotes");

    // Connect events from database manager to know, when we need to update quote widgets
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnInsertEvent,
            this, &QuotesWidget::UpdateQuotes);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnUpdateEvent,
            this, &QuotesWidget::UpdateQuotes);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnDeleteEvent,
            this, &QuotesWidget::UpdateQuotes);
}

///////////////////////////////////////////////////////////////////////////

QuotesWidget::~QuotesWidget()
{

}

///////////////////////////////////////////////////////////////////////////

void QuotesWidget::UpdateQuotes(const QString& tableName)
{
    // If table name is that, then we can handle it
    if (tableName == "Quotes")
    {
        // Get last quote number
        std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(number)");
        if (numberQuery != NULL)
        {
            numberQuery->first();
            int maxValue = numberQuery->value(0).toInt();
            // If we do not have enough widgets, create them
            if (maxValue > _layout->count())
            {
                for (int i = _layout->count()+1; i <= maxValue; ++i)
                {
                    SingleQuoteWidget* newQuote = new SingleQuoteWidget(this);
                    _layout->addWidget(newQuote);
                }
            }
            // If we have more widgets than quotes, then delete redundant widgets
            else if (_layout->count() > maxValue)
            {
                for (int i = _layout->count(); i > maxValue; --i)
                {
                    QLayoutItem* item = _layout->itemAt(i-1);
                    if (item != NULL)
                    {
                        _layout->removeItem(item);
                        delete item->widget();
                        delete item;
                    }
                }
            }

            // Update quote data in widgets
            std::shared_ptr<QSqlQuery> query = DB_SELECT("Quotes",
                                                         "number, quote",
                                                         "number > 0 ORDER BY number ASC");
            if (query != NULL)
            {
                while (query->next())
                {
                    // Get layout item
                    QLayoutItem* item = _layout->itemAt(query->value("number").toInt() - 1);
                    if (item != NULL)
                    {
                        SingleQuoteWidget* quoteWidget = dynamic_cast<SingleQuoteWidget*>(item->widget());
                        // Update widget data
                        if (quoteWidget != NULL)
                        {
                            quoteWidget->SetQuoteNumber(query->value("number").toString());
                            quoteWidget->SetQuoteText(query->value("quote").toString());
                        }
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
