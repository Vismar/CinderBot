/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "QuoteWindow.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Database/QuoteDBHelper.hpp"

using namespace Ui::Quote;
using namespace Utils::Database;

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

    // Initialize create quote widget
    _createQuoteWidget = new CreateQuoteWidget(this);
    _layout->addWidget(_createQuoteWidget);

    // Initialize quotes container
    _quotesWidget = new QuotesWidget(this);
    _layout->addWidget(_quotesWidget);

    // Connect events from QuoteDBHelper to know, when we need to update quote widgets
    connect(&QuoteDBHelper::Instance(), &QuoteDBHelper::QuoteAdded,
            this, &QuoteWindow::AddQuote);
    connect(&QuoteDBHelper::Instance(), &QuoteDBHelper::QuoteDeleted,
            this, &QuoteWindow::DeleteQuote);
    connect(&QuoteDBHelper::Instance(), &QuoteDBHelper::QuoteUpdated,
            this, &QuoteWindow::UpdateQuotes);

    UpdateQuotes(0);
    this->resize(this->minimumSize());
}

///////////////////////////////////////////////////////////////////////////

QuoteWindow::~QuoteWindow() { }

///////////////////////////////////////////////////////////////////////////

void QuoteWindow::AddQuote(int id)
{
    _quotesWidget->AddId(id);
}

///////////////////////////////////////////////////////////////////////////

void QuoteWindow::DeleteQuote(int id)
{
    _quotesWidget->DeleteId(id);
}

///////////////////////////////////////////////////////////////////////////

void QuoteWindow::UpdateQuotes(int quoteNumber)
{
    Q_UNUSED(quoteNumber);

    _quotesWidget->UpdateIds(QuoteDBHelper::GetQuoteIds());
}

///////////////////////////////////////////////////////////////////////////
