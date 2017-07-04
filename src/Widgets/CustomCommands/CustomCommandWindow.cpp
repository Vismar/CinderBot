/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCommandWindow.hpp"
#include "Utils/DatabaseManager.hpp"
#include "Widgets/CustomCommands/CreateCustomCommandWindow.hpp"
#include "Widgets/CustomCommands/EditCustomCommandWindow.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

CustomCommandWindow::CustomCommandWindow(QWidget *parent) : QMainWindow(parent)
{
    // Initialize dialog window
    this->setWindowTitle("Custom commands");
    this->setMinimumSize(350, 330);
    // Initialize widgets
    QWidget* container = new QWidget();
    this->setCentralWidget(container);
    _mainLayout = new QVBoxLayout(container);
    _mainLayout->setAlignment(Qt::AlignTop);
    _createButton = new QPushButton("Create");
    _commandList = new ListCustomCommandWidget();
    _mainLayout->addWidget(_createButton);
    _mainLayout->addWidget(_commandList);

    // Connect events
    connect(_createButton, &QPushButton::clicked,
            this, &CustomCommandWindow::_CreateCommand);


    // Connect events from database manager to know, when we need to update commands
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnInsertEvent,
            this, &CustomCommandWindow::_UpdateCommands);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnUpdateEvent,
            this, &CustomCommandWindow::_UpdateCommands);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnDeleteEvent,
            this, &CustomCommandWindow::_UpdateCommands);

    _UpdateCommands("CustomCommands");
}

///////////////////////////////////////////////////////////////////////////

CustomCommandWindow::~CustomCommandWindow() {}

///////////////////////////////////////////////////////////////////////////

void CustomCommandWindow::_CreateCommand()
{
    CreateCustomCommandWindow *createWindow = new CreateCustomCommandWindow();
    createWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(createWindow, &CreateCustomCommandWindow::NewCommand,
            this, &CustomCommandWindow::_EditNewCommand);
    createWindow->exec();
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandWindow::_UpdateCommands(const QString &tableName)
{
    if (tableName == "CustomCommands")
    {
        DB_QUERY_PTR query = DB_SELECT("CustomCommands", "Id");
        if (query != nullptr)
        {
            QVector<int> cmdIds;
            while (query->next())
            {
                cmdIds.append(query->value("Id").toInt());
            }
            _commandList->UpdateIds(cmdIds);
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandWindow::_EditNewCommand(const QString &cmdName)
{
    EditCustomCommandWindow *editWindow = new EditCustomCommandWindow();
    editWindow->setAttribute(Qt::WA_DeleteOnClose);
    editWindow->LoadCommandData(cmdName);
    editWindow->exec();
}

///////////////////////////////////////////////////////////////////////////
