/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "SingleQuoteWidget.hpp"
#include <QAbstractTextDocumentLayout>
#include <Utils/DatabaseManager.hpp>

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

SingleQuoteWidget::SingleQuoteWidget(QWidget* parent) : QFrame(parent)
{
    // For borders
    this->setFrameShape(QFrame::Box);

    // Quote number
    _quoteNumber = new QLabel(this);
    _quoteNumber->setFixedHeight(23);

    // Quote text
    _quoteText = new QTextEdit(this);
    _quoteText->setLineWrapMode(QTextEdit::WidgetWidth);
    _quoteText->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    _quoteText->setReadOnly(true);
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

void SingleQuoteWidget::SetQuoteNumber(const QString& number)
{
    _quoteNumber->setText(number);
}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::SetQuoteText(const QString& text)
{
    _quoteText->setText(text);
}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::_AdjustMinimumSize(const QSizeF& size)
{
    // Sets minimum size of widget, prevents it from squeezing
    _quoteText->setMinimumHeight(size.height() + 2 * _quoteText->frameWidth());
    setMinimumHeight(size.height() + 15);
    // Sets maximum size of widget, prevents it from stretching
    _quoteText->setMaximumHeight(size.height() + 2 * _quoteText->frameWidth());
    setMaximumHeight(size.height() + 15);

}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::_HandleEditSaveButton()
{
    // If button in "edit" state, turn edit on and change button state to "save"
    if (_quoteEditSave->text() == "Edit")
    {
        _quoteText->setReadOnly(false);
        _quoteEditSave->setText("Save");
        _quoteEditSave->setStyleSheet("QPushButton {background-color: rgb(155,200,155)}"
                                      "QPushButton:hover {background-color: rgb(155,230,155)}");
    }
    // If button in "save" state, turn edit off and change button state to "edit"
    // and save new quote text if it was really changed
    else if (_quoteEditSave->text() == "Save")
    {
        _quoteText->setReadOnly(true);
        _quoteEditSave->setText("Edit");
        _quoteEditSave->setStyleSheet("");
        std::shared_ptr<QSqlQuery> query = DB_SELECT("Quotes",
                                                     "Quote",
                                                     QString("number = %1").arg(_quoteNumber->text()));
        if (query != NULL)
        {
            query->first();
            // Save new quote text if was changed
            if (query->value(0).toString() != _quoteText->toPlainText())
            {
                DB_UPDATE("Quotes",
                          QString("Quote = '%1'").arg(_quoteText->toPlainText()),
                          QString("Number = %1").arg(_quoteNumber->text()));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void SingleQuoteWidget::_HandleDeleteButton()
{
    DB_DELETE("Quotes", QString("Number = %1").arg(_quoteNumber->text()));
}

///////////////////////////////////////////////////////////////////////////
