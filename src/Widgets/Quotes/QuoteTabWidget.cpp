#include "QuoteTabWidget.hpp"

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
}

///////////////////////////////////////////////////////////////////////////

QuoteTabWidget::~QuoteTabWidget()
{

}

///////////////////////////////////////////////////////////////////////////
