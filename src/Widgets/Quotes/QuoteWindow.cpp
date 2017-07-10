/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "QuoteWindow.hpp"
#include "Utils/DatabaseManager.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

QuoteWindow::QuoteWindow(QWidget* parent) : QWidget(parent, Qt::Window)
{
    // Initialize dialog window
    this->setWindowTitle("Quotes");
    this->setMinimumSize(450, 330);
    // Initialize layout
    //QWidget* container = new QWidget();
    //this->setCentralWidget(container);
    _layout = new QVBoxLayout(this);
    _layout->setMargin(10);
    _layout->setAlignment(Qt::AlignTop);

    // Initialize create quote iwdget
    _createQuoteWidget = new CreateQuoteWidget(this);
    _layout->addWidget(_createQuoteWidget);

    // Initialize quotes container
    _quotesWidget = new QuotesWidget(this);
    _layout->addWidget(_quotesWidget);

    // Connect events from database manager to know, when we need to update quote widgets
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnInsertEvent,
            this, &QuoteWindow::UpdateQuotes);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnUpdateEvent,
            this, &QuoteWindow::UpdateQuotes);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnDeleteEvent,
            this, &QuoteWindow::UpdateQuotes);
    UpdateQuotes("Quotes");
    this->resize(this->minimumSize());
}

///////////////////////////////////////////////////////////////////////////

QuoteWindow::~QuoteWindow() { }

///////////////////////////////////////////////////////////////////////////

void QuoteWindow::UpdateQuotes(const QString &tableName)
{
    // If table name is that, then we can handle it
    if (tableName == "Quotes")
    {
        std::shared_ptr<QSqlQuery> query = DB_SELECT("Quotes","Id", "Number>0");
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
