/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "BaseChatCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"

using namespace Command;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

BaseChatCommand::BaseChatCommand() : QObject(0)
{
    _Clear();
}

///////////////////////////////////////////////////////////////////////////

BaseChatCommand::~BaseChatCommand() { }

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::Execute(const ChatMessage &message, ChatAnswer &answer)
{
    bool result(false);

    if (_IsAnswerAllowed(message.GetType()))
    {
        QString messageInLowerCase = message.GetMessage().toLower();
        // Check if command was used properly (no additional symbols after it)
        if (messageInLowerCase.contains(_name+' ') ||
            ((messageInLowerCase.size() == _name.size()) && (messageInLowerCase.contains(_name))))
        {
            // If command should return random answer
            if (_isRandom)
            {
                _GetRandomAnswer(message, answer);
                result = !answer.GetAnswers().isEmpty();
            }
            // If answer will be specified by some conditions
            else
            {
                _GetAnswer(message, answer);
                result = !answer.GetAnswers().isEmpty();
            }
        }
        if (result)
        {
            _ReplacePlaceHolders(message, answer.GetAnswers());
            // Update last time used
            _lastTimeUsed = QDateTime::currentDateTime();
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

const QString& BaseChatCommand::GetCommandName() const
{
    return _name;
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_Clear()
{
    _workInWhisper = true;
    _workInChat = true;
    _isRandom = false;
    _name.clear();
    _cooldown.setHMS(0, 0, 0, 0);
    _moderatorOnly = false;
    _price = 0;
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_ReplacePlaceHolders(const ChatMessage &message, QStringList &answer)
{
    _AddAuthorName(answer, message.GetAuthor());
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_AddAuthorName(QStringList &answer, const QString &author)
{
    for (auto iter = answer.begin(); iter != answer.end(); ++iter)
    {
        (*iter).replace("@", author);
    }
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_TakeDefaultPriceFromUser(int userID) const
{
    UserDataDBHelper::GiveCurrencyToUser(-_price, userID);
}

///////////////////////////////////////////////////////////////////////////

void BaseChatCommand::_TakePriceFromUser(int userID, int price)
{
    UserDataDBHelper::GiveCurrencyToUser(-price, userID);
}

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::_IsAnswerAllowed(MessageType msgType)
{
    bool allowed(false);
    switch (msgType)
    {
    case BITS:
    case PRIVMSG:
        allowed = _workInChat;
        break;
    case WHISPER:
        allowed = _workInWhisper;
        break;
    default:
        break;
    }

    return allowed;
}

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::_CheckCovenant(int userID)
{
    return UserDataDBHelper::IsUserInCovenant(_covenant, userID);
}

///////////////////////////////////////////////////////////////////////////

bool BaseChatCommand::_CheckCurrency(int userID)
{
    bool currencyIsOk(true);
    // Get user currency value
    int userCurrency = UserDataDBHelper::GetUserParameter(UserDataParameter::Currency, userID).toInt();
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
    bool result(false);
    if (userIsModerator)
    {
        result = true;
    }
    else if (!_moderatorOnly)
    {
        result = true;
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
