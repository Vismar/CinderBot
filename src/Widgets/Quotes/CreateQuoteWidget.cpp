#include "CreateQuoteWidget.hpp"
#include <QAbstractTextDocumentLayout>
#include <Utils/DatabaseManager.hpp>
#include <QApplication>

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

CreateQuoteWidget::CreateQuoteWidget(QWidget* parent) : QFrame(parent)
{
    // Inititalize layout
    QWidget* container = new QWidget;
    _layout = new QHBoxLayout(container);
    _layout->setMargin(10);
    _layout->setAlignment(Qt::AlignLeft);
    this->setLayout(_layout);

    // Initialize add button
    _addButton = new QPushButton(this);
    _addButton->setText("Add");
    _addButton->setFixedHeight(22);
    _layout->addWidget(_addButton);
    connect(_addButton, &QPushButton::clicked,
            this, &CreateQuoteWidget::_AddQuote);

    // Inititalize text field
    _quoteText = new QTextEdit(this);
    _layout->addWidget(_quoteText);

    // Connect event to proper size change
    connect(_quoteText->document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged,
            this, &CreateQuoteWidget::_AdjustMinimumSize);
}

///////////////////////////////////////////////////////////////////////////

CreateQuoteWidget::~CreateQuoteWidget()
{

}

///////////////////////////////////////////////////////////////////////////

void CreateQuoteWidget::_AdjustMinimumSize(const QSizeF& size)
{
    // Sets minimum size of widget, prevents it from squeezing
    _quoteText->setMinimumHeight(size.height() + 2 * _quoteText->frameWidth());
    setMinimumHeight(size.height() + 15);
    // Sets maximum size of widget, prevents it from stretching
    _quoteText->setMaximumHeight(size.height() + 2 * _quoteText->frameWidth());
    setMaximumHeight(size.height() + 15);

}

///////////////////////////////////////////////////////////////////////////

void CreateQuoteWidget::_AddQuote()
{
    // If user typed something here, then we can add new quote
    if (!_quoteText->toPlainText().isEmpty())
    {
        //  Get last quote number
        std::shared_ptr<QSqlQuery> numberQuery = DB_SELECT("Quotes", "MAX(number)");
        if (numberQuery != NULL)
        {
            numberQuery->first();
            int newMaxValue = numberQuery->value(0).toInt() + 1;
            // If quote was added, clear text field
            if (DB_INSERT("Quotes", QString("NULL, '%1', %2").arg(_quoteText->toPlainText()).arg(newMaxValue)))
            {
                _quoteText->clear();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////