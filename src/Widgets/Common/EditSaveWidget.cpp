/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EditSaveWidget.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include <QAbstractTextDocumentLayout>

using namespace Ui::Common;

///////////////////////////////////////////////////////////////////////////

EditSaveWidget::EditSaveWidget(QWidget* parent) : QFrame(parent)
{
    // For borders
    this->setFrameShape(QFrame::Box);

    // Text
    _text = new EnhTextEdit(this);
    _text->setLineWrapMode(EnhTextEdit::WidgetWidth);
    _text->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    _text->SetEditable(false);
    connect(_text->document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged,
            this, &EditSaveWidget::_AdjustMinimumSize);

    // Edit/Save button
    _editSaveButton = new QPushButton(this);
    _editSaveButton->setText("Edit");
    _editSaveButton->setFixedHeight(22);
    _editState = true;
    connect(_editSaveButton, &QPushButton::clicked,
            this, &EditSaveWidget::_HandleEditSaveButton);

    // Quote delete button
    _deleteButton = new QPushButton(this);
    _deleteButton->setText("Delete");
    _deleteButton->setFixedHeight(22);
    connect(_deleteButton, &QPushButton::clicked,
            this, &EditSaveWidget::_HandleDeleteButton);

    // Layout initialization
    _layout = new QHBoxLayout(this);
    _layout->setMargin(5);
    _layout->addWidget(_text);
    _layout->addWidget(_editSaveButton, 0, Qt::AlignTop);
    _layout->addWidget(_deleteButton, 0, Qt::AlignTop);
}

///////////////////////////////////////////////////////////////////////////

EditSaveWidget::~EditSaveWidget() {}

///////////////////////////////////////////////////////////////////////////

void EditSaveWidget::SetId(int id)
{
    _id = id;
}

///////////////////////////////////////////////////////////////////////////

void EditSaveWidget::SetText(const QString &text) const
{
    _text->setText(text);
}

///////////////////////////////////////////////////////////////////////////

void EditSaveWidget::_AdjustMinimumSize(const QSizeF &size)
{
    // Sets minimum size of widget, prevents it from squeezing
    _text->setMinimumHeight(size.height() + 2 * _text->frameWidth());
    setMinimumHeight(size.height() + 15);
    // Sets maximum size of widget, prevents it from stretching
    _text->setMaximumHeight(size.height() + 2 * _text->frameWidth());
    setMaximumHeight(size.height() + 15);

}

///////////////////////////////////////////////////////////////////////////

void EditSaveWidget::_HandleEditSaveButton()
{
    // If button in "edit" state, turn edit on and change button state to "save"
    if (_editState)
    {
        _text->SetEditable(true);
        _editSaveButton->setText("Save");
        _editSaveButton->setStyleSheet("QPushButton {background-color: rgb(155,200,155)}"
                                       "QPushButton:hover {background-color: rgb(155,230,155)}");
        _editState = !_editState;
    }
    // If button in "save" state, turn edit off and change button state to "edit" and save new text
    else if (!_editState)
    {
        _text->SetEditable(false);
        _editSaveButton->setText("Edit");
        _editSaveButton->setStyleSheet("");
        _editState = !_editState;
        _SaveEntry();
    }
}

///////////////////////////////////////////////////////////////////////////

void EditSaveWidget::_HandleDeleteButton()
{
    _DeleteEntry();
}

///////////////////////////////////////////////////////////////////////////
