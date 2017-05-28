/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "BotAI.hpp"
#include <Utils/UserData/UserData.hpp>
#include <Utils/Config/ConfigurationManager.hpp>
#include <Utils/Config/ConfigurationParameters.hpp>
#include <Utils/UserData/RealTimeUserData.hpp>
/*** Command lists ***/
#include <AI/ChatCommands/CustomCommandList.hpp>
#include <AI/ChatCommands/UserDataCommandList.hpp>
#include <AI/ChatCommands/CovenantCommands/CovenantCommandList.hpp>
#include <AI/ChatCommands/QuoteCommands/QuoteCommandList.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

BotAI::BotAI(QObject* parent) : QObject(parent)
{
    // Load IgnoreList
    ConfigurationManager& configMng = ConfigurationManager::Instance();
    QString param;
    // Fill ignore list
    if (configMng.GetStringParam(CFGS_IGNORE, param))
    {
        _ignoreList = param.split(',');
    }
    if (configMng.GetStringParam(CFGP_LOGIN_NAME, param))
    {
        // If botname not added to the list, add it
        if (!_ignoreList.contains(param))
        {
            _ignoreList += param;
        }
    }

    /* Load commands */
    // Predefined commands
    _chatCommands.push_back(new UserDataCommandList());
    _chatCommands.push_back(new CovenantCommandList());
    _chatCommands.push_back(new QuoteCommandList());
    // Custom commands for all users and covenants
    _chatCommands.push_back(new CustomCommandList());

    // Connect and start currency timer
    connect(&_currencyTimer, &QTimer::timeout,
            this, &BotAI::GiveCurrencyOnTimer);
    param = "60000"; // default value
    configMng.GetStringParam(CFGP_CURRECY_TIMER, param);
    int timeValue = param.toInt();
    if (timeValue > 0)
    {
        _currencyTimer.start(timeValue);
    }
    else
    {
        _currencyTimer.start(60000); //default value
    }
}

///////////////////////////////////////////////////////////////////////////

BotAI::~BotAI()
{
    for (int i = 0; i < _chatCommands.size(); ++i)
    {
        delete _chatCommands[i];
    }
}

///////////////////////////////////////////////////////////////////////////

void BotAI::ReadNewMessage(ChatMessage message, bool botMessage)
{
    // Update user data
    _UpdateUserData(message);
    // If new message is not created by bot, parse it.
    if (!botMessage)
    {
        QString answer;

        for (int i = 0; i < _chatCommands.size(); ++i)
        {
            // If we found a command, emit event with result and break the loop
            if (_chatCommands[i]->TryGetAnswer(message, answer))
            {
                emit NewBotMessage(answer);
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void BotAI::GiveCurrencyOnTimer()
{
    QStringList userList = RealTimeUserData::Instance()->GetUserList();
    for (int i = 0; i < userList.count(); ++i)
    {
        if (!_ignoreList.contains(userList[i]))
        {
            // Add currency to user
            QString tempString = "1"; // default value
            ConfigurationManager::Instance().GetStringParam(CFGP_CURRENCY_OVER_TIME, tempString);
            _AddCurrency(userList[i], tempString.toInt());
        }
    }
}

///////////////////////////////////////////////////////////////////////////

bool BotAI::_CheckIgnoreList(const QString& userName)
{
    return _ignoreList.contains(userName.toLower());
}

///////////////////////////////////////////////////////////////////////////

void BotAI::_UpdateUserData(const ChatMessage& message)
{
    // If user is not in ignore list, update info
    if (!(_CheckIgnoreList(message.GetAuthor()) || (_CheckIgnoreList(message.GetRealName()))))
    {
        QString tempString = "1";
        _IncrementMessageCounter(message.GetRealName());
        // Add currency for message
        ConfigurationManager::Instance().GetStringParam(CFGP_CURRENCY_PER_MSG, tempString);
        _AddCurrency(message.GetRealName(), tempString.toInt());
    }
}

///////////////////////////////////////////////////////////////////////////

void BotAI::_IncrementMessageCounter(const QString& userName)
{
    QString param;
    // Get message counter
    param = UD_GET_PARAM(userName, UDP_Messages);
    // Increment counter
    param = QString::number(param.toInt() + 1);
    // Set new value
    UD_UPDATE(userName, UDP_Messages, param);
}

///////////////////////////////////////////////////////////////////////////

void BotAI::_AddCurrency(const QString& userName, const int value)
{
    QString param;
    // Get currency value
    param = UD_GET_PARAM(userName, UDP_Currency);
    // Update value
    param = QString::number(param.toInt() + value);
    // Set new value
    UD_UPDATE(userName, UDP_Currency, param);
}

///////////////////////////////////////////////////////////////////////////
