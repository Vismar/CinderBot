/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EditCustomCommandWindow.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Ui::CustomCommand;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

EditCustomCommandWindow::EditCustomCommandWindow(QWidget *parent) : QDialog(parent)
{
    // Initialize dialog window
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedWidth(820);
    this->setMinimumHeight(331);
    this->setWindowTitle("Edit custom command");
    // Initialize widgets
    _mainLayout = new QHBoxLayout(this);
    _mainLayout->setMargin(0);
    // Left part of window
    _leftPart = new QFrame();
    _leftPart->setFrameShape(QFrame::Box);
    _leftLayout = new QVBoxLayout(_leftPart);
    _leftLayout->setAlignment(Qt::AlignTop);
    // Custom command parameters
    _parameters = new ParametersCustomCommandWidget(this);
    _parameters->SetCommandNameChangeable(false);
    // New answer widgets
    _newAnswer = new QPlainTextEdit(this);
    _newAnswer->setFixedWidth(300);
    _newAnswer->setPlaceholderText("Type here your new answer");
    _addButton = new QPushButton("Add", this);
    _addButton->setFixedSize(300, 30);
    _addButton->setEnabled(false);
    // Save button
    _saveButton = new QPushButton("Save",this);
    _saveButton->setFixedSize(300,50);
    _answers = new AnswersCustomCommandWidget(this);
    _answers->setFixedWidth(493);
    // Add widgets to layout
    _leftLayout->addWidget(_parameters);
    _leftLayout->addWidget(_newAnswer);
    _leftLayout->addWidget(_addButton);
    _leftLayout->addWidget(_saveButton);
    _mainLayout->addWidget(_leftPart);
    _mainLayout->addWidget(_answers);
    this->resize(this->minimumSize());

    // Connect widget events
    connect(_newAnswer, &QPlainTextEdit::textChanged,
            this, &EditCustomCommandWindow::_CheckAnswerField);
    connect(_addButton, &QPushButton::clicked,
            this, &EditCustomCommandWindow::_OnAddButton);
    connect(_saveButton, &QPushButton::clicked,
            this, &EditCustomCommandWindow::_OnSaveButton);

    // Connect events from database manager to know, when we need to update answers
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnInsertEvent,
            this, &EditCustomCommandWindow::_UpdateAnswers);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnUpdateEvent,
            this, &EditCustomCommandWindow::_UpdateAnswers);
    connect(&DatabaseManager::Instance(), &DatabaseManager::OnDeleteEvent,
            this, &EditCustomCommandWindow::_UpdateAnswers);
}

///////////////////////////////////////////////////////////////////////////

EditCustomCommandWindow::~EditCustomCommandWindow() { }

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::LoadCommandData(const QString &cmdName)
{
    DB_QUERY_PTR paramQuery = DB_SELECT("CustomCommands", "*", QString("Name='%1'").arg(cmdName));
    DB_QUERY_PTR answersQuery = DB_SELECT("CustomCommandAnswers", "Id", QString("Name='%1'").arg(cmdName));

    if ((paramQuery != nullptr) && (answersQuery != nullptr))
    {
        if (paramQuery->first())
        {
            _parameters->SetCommandName(paramQuery->value("Name").toString());
            _parameters->SetCooldown(QTime::fromString(paramQuery->value("Cooldown").toString(), "h:m:s"));
            _parameters->SetModOnly(paramQuery->value("ModeratorOnly").toBool());
            _parameters->SetPrice(paramQuery->value("Price").toInt());
            _parameters->SetCovenant(paramQuery->value("Covenant").toString());

            QVector<int> answerIds;
            while (answersQuery->next())
            {
                answerIds.append(answersQuery->value("Id").toInt());
            }
            _answers->UpdateIds(answerIds);

            this->setWindowTitle(QString("[%1] Edit custom command").arg(_parameters->GetCommandName()));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::_CheckAnswerField()
{
    if (_newAnswer->toPlainText().length() > 0)
    {
        _addButton->setEnabled(true);
    }
    else
    {
        _addButton->setEnabled(false);
    }
}

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::_OnAddButton()
{
    DB_INSERT("CustomCommandAnswers",
              QString("NULL, '%1', '%2'").arg(_parameters->GetCommandName()).arg(_newAnswer->toPlainText()));
    _newAnswer->clear();
}

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::_OnSaveButton()
{
    QString values = "Cooldown=':cooldown', ModeratorOnly=:mod, Price=:price, Covenant=':cov'";
    values.replace(":cooldown", _parameters->GetCooldown().toString("h:m:s"));
    values.replace(":mod", QString::number((int)_parameters->GetModOnly()));
    values.replace(":price", QString::number(_parameters->GetPrice()));
    QString covenant = _parameters->GetCovenant();
    if (covenant == "None")
    {
        covenant = "Viewer";
    }
    values.replace(":cov", covenant);
    DB_UPDATE("CustomCommands", values, QString("Name='%1'").arg(_parameters->GetCommandName()));
    close();
}

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::_UpdateAnswers(const QString &tableName)
{
    if (tableName == "CustomCommandAnswers")
    {
        int page = _answers->GetCurrentPage();
        DB_QUERY_PTR answersQuery = DB_SELECT("CustomCommandAnswers",
                                              "Id",
                                              QString("Name='%1'").arg(_parameters->GetCommandName()));
        if (answersQuery != nullptr)
        {
            QVector<int> answerIds;
            while (answersQuery->next())
            {
                answerIds.append(answersQuery->value("Id").toInt());
            }
            _answers->UpdateIds(answerIds);
        }
        _answers->SelectPage(page);
    }
}

///////////////////////////////////////////////////////////////////////////
