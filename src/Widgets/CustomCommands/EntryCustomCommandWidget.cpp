/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EntryCustomCommandWidget.hpp"
#include "Widgets/CustomCommands/EditCustomCommandWindow.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Ui::CustomCommand;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

EntryCustomCommandWidget::EntryCustomCommandWidget(QWidget* parent) : QFrame(parent)
{
    // For borders
    this->setFrameShape(QFrame::Box);

    // Command name
    _cmdName = new QLineEdit(this);
    _cmdName->setReadOnly(true);

    // Edit button
    _editButton = new QPushButton(this);
    _editButton->setText("Edit");
    _editButton->setFixedHeight(22);
    connect(_editButton, &QPushButton::clicked,
            this, &EntryCustomCommandWidget::_HandleEditButton);

    // Quote delete button
    _deleteButton = new QPushButton(this);
    _deleteButton->setText("Delete");
    _deleteButton->setFixedHeight(22);
    connect(_deleteButton, &QPushButton::clicked,
            this, &EntryCustomCommandWidget::_HandleDeleteButton);

    // Layout initialization
    _layout = new QHBoxLayout(this);
    _layout->setMargin(5);
    _layout->addWidget(_cmdName, 0, Qt::AlignTop);
    _layout->addWidget(_editButton, 0, Qt::AlignTop);
    _layout->addWidget(_deleteButton, 0, Qt::AlignTop);
}

///////////////////////////////////////////////////////////////////////////

EntryCustomCommandWidget::~EntryCustomCommandWidget() {}

///////////////////////////////////////////////////////////////////////////

void EntryCustomCommandWidget::SetCmdName(const QString &cmdName)
{
    _cmdName->setText(cmdName);
}


///////////////////////////////////////////////////////////////////////////

void EntryCustomCommandWidget::_HandleEditButton()
{
    EditCustomCommandWindow *dialog = new EditCustomCommandWindow();
    dialog->LoadCommandData(_cmdName->text());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}

///////////////////////////////////////////////////////////////////////////

void EntryCustomCommandWidget::_HandleDeleteButton()
{
    DB_DELETE("CustomCommandAnswers", QString("Name='%1'").arg(_cmdName->text()));
    DB_DELETE("CustomCommands", QString("Name='%1'").arg(_cmdName->text()));
}

///////////////////////////////////////////////////////////////////////////
