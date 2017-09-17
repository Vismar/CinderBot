/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "SingleQuoteWidget.hpp"
#include "Utils/Database/QuoteDBHelper.hpp"
#include <QAbstractTextDocumentLayout>

using namespace Ui::Quote;
using namespace Ui::Common;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

SingleQuoteWidget::SingleQuoteWidget(QWidget* parent) : QFrame(parent)
{
    // For borders
    this->setFrameShape(QFrame::Box);

    // Quote number
    _quoteNumber = new QLabel(this);
    _quoteNumber->setFixedHeight(23);

    // Quote text
    _quoteText = new EnhTextEdit(this);
    _quoteText->setLineWrapMode(QTextEdit::WidgetWidth);
    _quoteText->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    _quoteText->SetEditable(false);
    connect(_quoteText->document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged,
            this, &SingleQuoteWidget::_AdjustMinimumSize);

    // Quote edit/save button
    _quoteEditSave = new QPushButton(this);
    _quoteEditSave->setText("Edit");
    _quoteEditSave->setFixedHeight(22);
    connect(_quoteEditSave, &QPushButton::clicked,
            this, &SingleQuoteWidget::_HandleEditSaveButton);

    // Quote delete button
    _quoteDelete = new QPushButton(this);
    _quoteDelete->setText("Delete");
    _quoteDelete->setFixedHeight(22);
    connect(_quoteDelete, &QPushButton::clicked,
            this, &SingleQuoteWidget::_HandleDeleteButton);

    // Layout initialization
    _layout = new QHBoxLayout(this);
    _layout->setMargin(5);
    _layout->addWidget(_quoteNumber);
    _layout->setAlignment(_quoteNumber, Qt::AlignTop);
    _layout->addWidget(_quoteText);
    _layout->addWidget(_quoteEditSave);
    _layout->setAlignment(_quoteEditSave, Qt::AlignTop);
    _layout->addWidget(_quoteDelete);
    _layout->setAlignment(_quoteDelete, Qt::AlignTop);
}

///////////////////////////////////////////////////////////////////////////

SingleQuoteWidget::~SingleQuoteWidget() {}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::SetQuoteNumber(const QString &number) const
{
    _quoteNumber->setText(number);
}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::SetQuoteText(const QString &text) const
{
    _quoteText->setText(text);
}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::_AdjustMinimumSize(const QSizeF &size)
{
    // Sets minimum size of widget, prevents it from squeezing
    _quoteText->setMinimumHeight(size.height() + 2 * _quoteText->frameWidth());
    setMinimumHeight(size.height() + 15);
    // Sets maximum size of widget, prevents it from stretching
    _quoteText->setMaximumHeight(size.height() + 2 * _quoteText->frameWidth());
    setMaximumHeight(size.height() + 15);

}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::_HandleEditSaveButton() const
{
    // If button in "edit" state, turn edit on and change button state to "save"
    if (_quoteEditSave->text() == "Edit")
    {
        _quoteText->SetEditable(true);
        _quoteEditSave->setText("Save");
        _quoteEditSave->setStyleSheet("QPushButton {background-color: rgb(155,200,155)}"
                                      "QPushButton:hover {background-color: rgb(155,230,155)}");
    }
    // If button in "save" state, turn edit off and change button state to "edit"
    // and save new quote text if it was really changed
    else if (_quoteEditSave->text() == "Save")
    {
        _quoteText->SetEditable(false);
        _quoteEditSave->setText("Edit");
        _quoteEditSave->setStyleSheet("");

        QString currentQuote = QuoteDBHelper::Instance().GetQuote(_quoteNumber->text().toInt());

        // Save new quote text if it was changed
        if (!currentQuote.isEmpty() && (currentQuote != _quoteText->toPlainText()))
        {
            QuoteDBHelper::Instance().EditQuote(_quoteNumber->text().toInt(), _quoteText->toPlainText());
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::_HandleDeleteButton() const
{
    QuoteDBHelper::Instance().DeleteQuote(_quoteNumber->text().toInt());
}

///////////////////////////////////////////////////////////////////////////
