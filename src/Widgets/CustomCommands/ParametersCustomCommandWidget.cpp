/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ParametersCustomCommandWidget.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include <QStringList>

using namespace Ui::CustomCommand;
using namespace Ui::Common;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

ParametersCustomCommandWidget::ParametersCustomCommandWidget(QWidget *parent) : QFrame(parent)
{
    // For borders
    this->setFixedSize(300, 170);
    this->setFrameShape(QFrame::Box);
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

ParametersCustomCommandWidget::~ParametersCustomCommandWidget() {}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::SetCommandNameChangeable(bool state) const
{
    _commandName->SetEditable(state);
}

///////////////////////////////////////////////////////////////////////////

QString ParametersCustomCommandWidget::GetCommandName() const
{
    return _commandName->text();
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::SetCommandName(const QString& cmdName) const
{
    _commandName->setText(cmdName);
}

///////////////////////////////////////////////////////////////////////////

QTime ParametersCustomCommandWidget::GetCooldown() const
{
    return _cooldown->time();
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::SetCooldown(const QTime& cooldown) const
{
    _cooldown->setTime(cooldown);
}

///////////////////////////////////////////////////////////////////////////

bool ParametersCustomCommandWidget::GetModOnly() const
{
    return (_moderatorOnly->checkState() == Qt::Checked);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::SetModOnly(bool modOnly) const
{
    if (modOnly)
    {
        _moderatorOnly->setCheckState(Qt::Checked);
    }
    else
    {
        _moderatorOnly->setCheckState(Qt::Unchecked);
    }
}

///////////////////////////////////////////////////////////////////////////

int ParametersCustomCommandWidget::GetPrice() const
{
    return _price->value();
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::SetPrice(int price) const
{
    _price->setValue(price);
}

///////////////////////////////////////////////////////////////////////////

QString ParametersCustomCommandWidget::GetCovenant() const
{
    return _covenant->currentText();
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::SetCovenant(const QString& covenant) const
{
    return _covenant->setCurrentText(covenant);
}

///////////////////////////////////////////////////////////////////////////

bool ParametersCustomCommandWidget::GetWorkInWhisper() const
{
    return (_workInWhisper->checkState() == Qt::Checked);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::SetWorkInWhisper(bool workInWhisper) const
{
    if (workInWhisper)
    {
        _workInWhisper->setCheckState(Qt::Checked);
    }
    else
    {
        _workInWhisper->setCheckState(Qt::Unchecked);
    }
}

///////////////////////////////////////////////////////////////////////////

bool ParametersCustomCommandWidget::GetWorkInChat() const
{
    return (_workInChat->checkState() == Qt::Checked);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::SetWorkInChat(bool workInChat) const
{
    if (workInChat)
    {
        _workInChat->setCheckState(Qt::Checked);
    }
    else
    {
        _workInChat->setCheckState(Qt::Unchecked);
    }
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_OnCmdNameChanged(const QString &text)
{
    // Set everything to lowercase
    {
        const QSignalBlocker blocker(_commandName);
        _commandName->setText(text.toLower());
    }    

    emit OnCommandNameFieldChanged(text);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_Initialize()
{
    _mainLayout = new QGridLayout(this);
    _mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    _InitializeCmdName();
    _InitializeCooldown();
    _InitializeModOnly();
    _InitializePrice();
    _InitializeCovenant();
    _InitializeWorkInWhisper();
    _InitializeWorkInChat();
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_InitializeCmdName()
{
    _commandNameLabel = new QLabel();
    _commandNameLabel->setText("Command");
    _mainLayout->addWidget(_commandNameLabel, 0, 0, Qt::AlignRight);
    _commandName = new EnhLineEdit();
    _commandName->setPlaceholderText("!command");
    _commandName->setMaxLength(25);
    _commandName->setFixedWidth(160);
    connect(_commandName, &EnhLineEdit::textChanged,
            this, &ParametersCustomCommandWidget::_OnCmdNameChanged);
    _mainLayout->addWidget(_commandName, 0, 1, Qt::AlignLeft);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_InitializeCooldown()
{
    _cooldownLabel = new QLabel();
    _cooldownLabel->setText("Cooldown(h:m:s)");
    _cooldownLabel->setToolTip("How often command can be executed?");
    _mainLayout->addWidget(_cooldownLabel, 1, 0, Qt::AlignRight);
    _cooldown = new QTimeEdit();
    _cooldown->setDisplayFormat("h:m:s");
    _cooldown->setFixedWidth(65);
    _mainLayout->addWidget(_cooldown, 1, 1, Qt::AlignLeft);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_InitializeModOnly()
{
    _modOnlyLabel = new QLabel();
    _modOnlyLabel->setText("Moderator only");
    _modOnlyLabel->setToolTip("Is this command for moderators only?");
    _mainLayout->addWidget(_modOnlyLabel, 2, 0, Qt::AlignRight);
    _moderatorOnly = new QCheckBox();
    _mainLayout->addWidget(_moderatorOnly, 2, 1, Qt::AlignLeft);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_InitializePrice()
{
    _priceLabel = new QLabel();
    _priceLabel->setText("Price");
    _priceLabel->setToolTip("How much currency user need to have to execute this command?");
    _mainLayout->addWidget(_priceLabel, 3, 0, Qt::AlignRight);
    _price = new QSpinBox();
    _price->setMaximum(99999);
    _price->setFixedWidth(55);
    _mainLayout->addWidget(_price, 3, 1, Qt::AlignLeft);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_InitializeCovenant()
{
    _covenantLabel = new QLabel();
    _covenantLabel->setText("Covenant");
    _covenantLabel->setToolTip("Which covenant should be able to execute this command? "
                               "If \"None\" is selected then anyone will be able to use it.");
    _mainLayout->addWidget(_covenantLabel, 4, 0, Qt::AlignRight);
    QStringList covList;
    covList.append("None");
    // Get covenant list
    DB_QUERY_PTR query = DB_SELECT("Covenants", "Name");
    if (query->exec())
    {
        while (query->next())
        {
            covList.append(query->value("Name").toString());
        }
    }
    _covenant = new QComboBox();
    _covenant->setEditable(false);
    _covenant->addItems(covList);
    _mainLayout->addWidget(_covenant);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_InitializeWorkInWhisper()
{
    _wokrInWhisperLabel = new QLabel();
    _wokrInWhisperLabel->setText("Work in /w");
    _wokrInWhisperLabel->setToolTip("Is this command can be executed by whisper message?");
    _mainLayout->addWidget(_wokrInWhisperLabel, 5, 0, Qt::AlignRight);
    _workInWhisper = new QCheckBox();
    _mainLayout->addWidget(_workInWhisper, 5, 1, Qt::AlignLeft);
}

///////////////////////////////////////////////////////////////////////////

void ParametersCustomCommandWidget::_InitializeWorkInChat()
{
    _wokrInChatLabel = new QLabel();
    _wokrInChatLabel->setText("Work in /chat");
    _wokrInChatLabel->setToolTip("Is this command can be executed by chat message?");
    _mainLayout->addWidget(_wokrInChatLabel, 6, 0, Qt::AlignRight);
    _workInChat = new QCheckBox();
    _mainLayout->addWidget(_workInChat, 6, 1, Qt::AlignLeft);
}

///////////////////////////////////////////////////////////////////////////
