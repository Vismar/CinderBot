/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CreateCustomCommandWindow.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"
#include <QMessageBox>

using namespace Ui::CustomCommand;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CreateCustomCommandWindow::CreateCustomCommandWindow(QWidget *parent) : QDialog(parent)
{
    // Initialize dialog window
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedSize(320, 230);
    this->setWindowTitle("Create custom command");
    // Initialize widgets
    _layout = new QVBoxLayout(this);
    _layout->setAlignment(Qt::AlignTop);
    _parameters = new ParametersCustomCommandWidget(this);
    _parameters->SetWorkInChat(true);
    _createButton = new QPushButton("Create",this);
    _createButton->setFixedSize(300,30);
    _createButton->setEnabled(false);
    _layout->addWidget(_parameters);
    _layout->addWidget(_createButton);

    // Connect events
    connect(_parameters, &ParametersCustomCommandWidget::OnCommandNameFieldChanged,
            this, &CreateCustomCommandWindow::_CheckCommandNameField);
    connect(_createButton, &QPushButton::clicked,
            this, &CreateCustomCommandWindow::_OnCreateButton);
}

///////////////////////////////////////////////////////////////////////////

CreateCustomCommandWindow::~CreateCustomCommandWindow() {}

///////////////////////////////////////////////////////////////////////////

void CreateCustomCommandWindow::_CheckCommandNameField(const QString &text) const
{
    if (text.isEmpty())
    {
        _createButton->setEnabled(false);
    }
    else
    {
        _createButton->setEnabled(true);
    }
}

///////////////////////////////////////////////////////////////////////////

void CreateCustomCommandWindow::_OnCreateButton()
{
    CmdParams cmdParams;
    // Set parameters
    cmdParams.Cooldown = _parameters->GetCooldown();
    cmdParams.ModeratorOnly = _parameters->GetModOnly();
    cmdParams.Price = _parameters->GetPrice();
    cmdParams.Covenant = _parameters->GetCovenant();
    if (cmdParams.Covenant == "None")
    {
        cmdParams.Covenant = "Viewer";
    }
    cmdParams.WorkInWhisper = _parameters->GetWorkInWhisper();
    cmdParams.WorkInChat = _parameters->GetWorkInChat();

    // Try to create command
    if (CustomCommandDBHelper::Instance().CreateCommand(CmdType::StreamerCmd, _parameters->GetCommandName(), cmdParams))
    {
        close();
        emit NewCommand(_parameters->GetCommandName());
    }
    // If command already exist (user command or covenant command), display message box
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Such command already exist!");
        msgBox.exec();
    }
}

///////////////////////////////////////////////////////////////////////////
