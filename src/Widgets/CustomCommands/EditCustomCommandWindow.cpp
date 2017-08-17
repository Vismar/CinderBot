/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EditCustomCommandWindow.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

using namespace Ui::CustomCommand;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

EditCustomCommandWindow::EditCustomCommandWindow(QWidget *parent) : QDialog(parent)
{
    // Initialize dialog window
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedWidth(820);
    this->setMinimumHeight(361);
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

    // Connect events from CustomCommandDBHelper to know, when we need to update answers
    connect(&CustomCommandDBHelper::Instance(), &CustomCommandDBHelper::CustomCmdAnswerAdded,
            this, &EditCustomCommandWindow::_AddAnswer);
    connect(&CustomCommandDBHelper::Instance(), &CustomCommandDBHelper::CustomCmdAnswerDeleted,
        this, &EditCustomCommandWindow::_DeleteAnswer);
}

///////////////////////////////////////////////////////////////////////////

EditCustomCommandWindow::~EditCustomCommandWindow() { }

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::LoadCommandData(const QString &cmdName)
{
    CmdParams cmdParams = CustomCommandDBHelper::Instance().GetAllParams(CmdType::StreamerCmd, cmdName);

    _parameters->SetCommandName(cmdName);
    _parameters->SetCooldown(cmdParams.Cooldown);
    _parameters->SetModOnly(cmdParams.ModeratorOnly);
    _parameters->SetPrice(cmdParams.Price);
    _parameters->SetCovenant(cmdParams.Covenant);
    _parameters->SetWorkInWhisper(cmdParams.WorkInWhisper);
    _parameters->SetWorkInChat(cmdParams.WorkInChat);

    QVector<int> answerIds = CustomCommandDBHelper::Instance().GetAnswers(CmdType::StreamerCmd, cmdName);
    _answers->UpdateIds(answerIds);

    this->setWindowTitle(QString("[%1] Edit custom command").arg(cmdName));
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
    CustomCommandDBHelper::Instance().AddAnswer(CmdType::StreamerCmd, _parameters->GetCommandName(), _newAnswer->toPlainText());
    _newAnswer->clear();
}

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::_OnSaveButton()
{
    CmdParams cmdParams;
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

    CustomCommandDBHelper::Instance().SetAllParams(CmdType::StreamerCmd, _parameters->GetCommandName(), cmdParams);
    close();
}

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::_LoadAnswers()
{
    QVector<int> answerIds = CustomCommandDBHelper::Instance().GetAnswers(CmdType::StreamerCmd, _parameters->GetCommandName());

    // Save page
    int page = _answers->GetCurrentPage();
    // Update ids
    _answers->UpdateIds(answerIds);
    // Select page which was recently selected
    _answers->SelectPage(page);
}

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::_AddAnswer(const QString &cmdName)
{
    // If answer was added to opened command, then update all answers in from list
    if (cmdName == _parameters->GetCommandName())
    {
        _LoadAnswers();
    }
}

///////////////////////////////////////////////////////////////////////////

void EditCustomCommandWindow::_DeleteAnswer(const QString &cmdName, int id)
{
    // If answer was deleted from opened command, then delete answer with specified id from list
    if (cmdName == _parameters->GetCommandName())
    {
        _answers->DeleteId(id);
    }
}

///////////////////////////////////////////////////////////////////////////
