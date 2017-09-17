/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CreateQuoteWidget.hpp"
#include "Utils/Database/QuoteDBHelper.hpp"
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QMessageBox>

using namespace Ui::Quote;
using namespace Ui::Common;
using namespace Utils::Database;

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
    _quoteText = new EnhTextEdit(this);
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

void CreateQuoteWidget::_AdjustMinimumSize(const QSizeF &size)
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
        // Add quote
        if (QuoteDBHelper::Instance().AddQuote(_quoteText->toPlainText()) > 0)
        {
            _quoteText->clear();
        }
        else
        {
            // Show error in message box
            QMessageBox msgBox;
            msgBox.setText("There is something goes wrong. Quote was not added. Please, provide feedback about such problem to the developer.");
            msgBox.exec();
        }
    }
}

///////////////////////////////////////////////////////////////////////////
