/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CreateCustomCommandWindow.hpp"
#include "Utils/DatabaseManager.hpp"
#include <QMessageBox>

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

CreateCustomCommandWindow::CreateCustomCommandWindow(QWidget *parent) : QDialog(parent)
{
    // Initialize dialog window
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedSize(320, 200);
    this->setWindowTitle("Create custom command");
    // Initialize widgets
    _layout = new QVBoxLayout(this);
    _layout->setAlignment(Qt::AlignTop);
    _parameters = new ParametersCustomCommandWidget(this);
    _createButton = new QPushButton("Create",this);
    _createButton->setFixedSize(300,30);
    _createButton->setEnabled(false);
    _layout->addWidget(_parameters);
    _layout->addWidget(_createButton);

    // Connect events
    connect(_parameters, &ParametersCustomCommandWidget::OnCommandNameFieldChanged,
            this, _CheckCommandNameField);
    connect(_createButton, &QPushButton::clicked,
            this, &CreateCustomCommandWindow::_OnCreateButton);
}

///////////////////////////////////////////////////////////////////////////

CreateCustomCommandWindow::~CreateCustomCommandWindow() {}

///////////////////////////////////////////////////////////////////////////

void CreateCustomCommandWindow::_CheckCommandNameField(const QString &text)
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
    DB_QUERY_PTR query = DB_SELECT("CustomCommands", "Id", QString("Name='%1'").arg(_parameters->GetCommandName()));
    if ((query != nullptr) && (query->first()))
    {
        QMessageBox msgBox;
        msgBox.setText("Such command already exist!");
        msgBox.exec();
    }
    else
    {
        QString values = "NULL, 'name', 'cooldown', mod, price, 'cov'";
        values.replace("name", _parameters->GetCommandName());
        values.replace("cooldown", _parameters->GetCooldown().toString());
        values.replace("mod", QString::number((_parameters->GetModOnly() ? 1 : 0)));
        values.replace("price", QString::number(_parameters->GetPrice()));
        QString covenant = _parameters->GetCovenant();
        if (covenant == "None")
        {
            covenant = "Viewer";
        }
        values.replace("cov", covenant);
        DB_INSERT("CustomCommands", values);
        close();
        emit NewCommand(_parameters->GetCommandName());
    }
}

///////////////////////////////////////////////////////////////////////////
