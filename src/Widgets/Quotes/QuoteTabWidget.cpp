/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "QuoteTabWidget.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

QuoteTabWidget::QuoteTabWidget(QWidget* parent) : QWidget(parent)
{
    // Initialize layout
    _layout = new QVBoxLayout();
    _layout->setMargin(10);
    _layout->setAlignment(Qt::AlignTop);
    this->setLayout(_layout);

    // Initialize create quote iwdget
    _createQuoteWidget = new CreateQuoteWidget(this);
    _layout->addWidget(_createQuoteWidget);

    // Initialize quotes container
    _quotesWidget = new QuotesWidget(this);
    _layout->addWidget(_quotesWidget);

    // Connect events from database manager to know, when we need to update quote widgets
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnInsertEvent,
            this, &QuoteTabWidget::UpdateQuotes);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnUpdateEvent,
            this, &QuoteTabWidget::UpdateQuotes);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnDeleteEvent,
            this, &QuoteTabWidget::UpdateQuotes);
    UpdateQuotes("Quotes");
}

///////////////////////////////////////////////////////////////////////////

QuoteTabWidget::~QuoteTabWidget() { }

///////////////////////////////////////////////////////////////////////////

void QuoteTabWidget::UpdateQuotes(const QString &tableName)
{
    // If table name is that, then we can handle it
    if (tableName == "Quotes")
    {
        std::shared_ptr<QSqlQuery> query = DB_SELECT("Quotes","Id");
        if (query != nullptr)
        {
            QVector<int> ids;
            while (query->next())
            {
                ids.push_back(query->value("Id").toInt());
            }
            _quotesWidget->UpdateIds(ids);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
