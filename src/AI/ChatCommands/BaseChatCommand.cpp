/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "BaseChatCommand.hpp"
#include <Utils/UserData/UserData.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

BaseChatCommand::BaseChatCommand()
{
    _Clear();
}

///////////////////////////////////////////////////////////////////////////

BaseChatCommand::~BaseChatCommand() { }

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::Execute(const ChatMessage& message, QString& answer)
{
    bool result(false);
    QString messageInLowerCase = message.GetMessage().toLower();
    // Check if command was used properly (no additional symbols after it)
    if (messageInLowerCase.contains(_name+' ') || messageInLowerCase.contains(_name+'\r'))
    {
        // If command should return random answer
        if (_isRandom)
        {
            _GetRandomAnswer(message, answer);
            result = !answer.isEmpty();
        }
        // If answer will be specified by some conditions
        else
        {
            _GetAnswer(message, answer);
            result = !answer.isEmpty();
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_GetRandomAnswer(const ChatMessage& message, QString& answer)
{
    if (_CheckModerationFlag(message.IsModerator()) &&
        _CheckCooldown() &&
        _CheckCurrency(message.GetRealName()) &&
        _CheckCovenant(message.GetRealName()))
    {
        // Update user currency value
        _TakeDefaultPriceFromUser(message.GetRealName());
        // Save time of exection
        _lastTimeUsed = QDateTime::currentDateTime();
        // Get random id for answer
        int id = qrand() % _answers.size();
        // Set returning answer
        answer = _answers.at(id);
    }
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_Clear()
{
    _isRandom = false;
    _name.clear();
    _answers.clear();
    _cooldown.setHMS(0, 0, 0, 0);
    _moderatorOnly = false;
    _price = 0;
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_ReplacePlaceHolders(const ChatMessage& message, QString& answer)
{
    _AddAuthorName(answer, message.GetAuthor());
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_AddAuthorName(QString& answer, const QString& author)
{
    answer.replace("@", author);
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_TakeDefaultPriceFromUser(const QString& userName)
{
    // Get user currency value
    int userCurrency = UD_GET_PARAM(userName, UDP_Currency).toInt();
    // Change user currency value
    userCurrency -= _price;
    QString newUserCurrencyValue = QString::number(userCurrency);
    UD_UPDATE(userName, UDP_Currency, newUserCurrencyValue);
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_TakePriceFromUser(const QString& userName, int price)
{
    // Get user currency value
    int userCurrency = UD_GET_PARAM(userName, UDP_Currency).toInt();
    // Change user currency value
    userCurrency -= price;
    QString newUserCurrencyValue = QString::number(userCurrency);
    UD_UPDATE(userName, UDP_Currency, newUserCurrencyValue);
}

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::_CheckCovenant(const QString& userName)
{
    bool covenantIsOk(true);

    // If covenant is specified, check covenant in which user in
    if (!_covenant.isEmpty())
    {
        QString userCovenantName = UD_GET_PARAM(userName, UDP_Covenant);
        // If users covenant is not the same as specified, then command cannot be executed
        if (_covenant != userCovenantName)
        {
            covenantIsOk = false;
        }
    }

    return covenantIsOk;
}

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::_CheckCurrency(const QString& userName)
{
    bool currencyIsOk(true);
    // Get user currency value
    QString tempUserCurrency = UD_GET_PARAM(userName, UDP_Currency);
    int userCurrency = tempUserCurrency.toInt();
    // Set user currency value to 0 if converting was failed
    if (userCurrency < 0)
    {
        userCurrency = 0;
    }
    // If user have not enough currency to execute command, then command cannot be executed
    if (userCurrency < _price)
    {
        currencyIsOk = false;
    }

    return currencyIsOk;
}

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::_CheckCooldown()
{
    // Get time when command can be executed
    QDateTime timeToUse = _lastTimeUsed.addMSecs(QTime(0,0,0,0).msecsTo(_cooldown));

    return (timeToUse < QDateTime::currentDateTime());
}

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::_CheckModerationFlag(bool userIsModerator)
{
    return (_moderatorOnly && userIsModerator);
}

///////////////////////////////////////////////////////////////////////////