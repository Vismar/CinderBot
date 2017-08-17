/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCommandWindow.hpp"
#include "Widgets/CustomCommands/CreateCustomCommandWindow.hpp"
#include "Widgets/CustomCommands/EditCustomCommandWindow.hpp"

using namespace Ui::CustomCommand;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CustomCommandWindow::CustomCommandWindow(QWidget *parent) : QWidget(parent, Qt::Window)
{
    // Initialize dialog window
    this->setWindowTitle("Custom commands");
    this->setMinimumSize(350, 330);
    // Initialize widgets
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setAlignment(Qt::AlignTop);
    _createButton = new QPushButton("Create");
    _commandList = new ListCustomCommandWidget();
    _mainLayout->addWidget(_createButton);
    _mainLayout->addWidget(_commandList);

    // Connect events
    connect(_createButton, &QPushButton::clicked,
            this, &CustomCommandWindow::_CreateCommand);

    // Connect events from CustomCommandDBHelper to know, when we need to update commands
    connect(&CustomCommandDBHelper::Instance(), &CustomCommandDBHelper::CustomCmdAdded,
            this, &CustomCommandWindow::_AddCommand);
    connect(&CustomCommandDBHelper::Instance(), &CustomCommandDBHelper::CustomCmdDeleted,
        this, &CustomCommandWindow::_DeleteCommand);

    _LoadCommands();
}

///////////////////////////////////////////////////////////////////////////

CustomCommandWindow::~CustomCommandWindow() {}

///////////////////////////////////////////////////////////////////////////

void CustomCommandWindow::_CreateCommand() const
{
    CreateCustomCommandWindow *createWindow = new CreateCustomCommandWindow();
    createWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(createWindow, &CreateCustomCommandWindow::NewCommand,
            this, &CustomCommandWindow::_EditNewCommand);
    createWindow->exec();
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandWindow::_AddCommand(CmdType cmdType, const QString &cmdName, int commandId) const
{
    Q_UNUSED(cmdName);
    if (cmdType == CmdType::StreamerCmd)
    {
        _commandList->AddId(commandId);
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandWindow::_DeleteCommand(CmdType cmdType, const QString & cmdName, int commandId) const
{
    Q_UNUSED(cmdName);
    if (cmdType == CmdType::StreamerCmd)
    {
        _commandList->DeleteId(commandId);
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandWindow::_EditNewCommand(const QString &cmdName) const
{
    EditCustomCommandWindow *editWindow = new EditCustomCommandWindow();
    editWindow->setAttribute(Qt::WA_DeleteOnClose);
    editWindow->LoadCommandData(cmdName);
    editWindow->exec();
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandWindow::_LoadCommands() const
{
    QVector<int> commandIds = CustomCommandDBHelper::Instance().GetCommandIds(CmdType::StreamerCmd);

    _commandList->UpdateIds(commandIds);
}

///////////////////////////////////////////////////////////////////////////
