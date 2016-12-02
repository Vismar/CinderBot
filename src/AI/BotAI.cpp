#include "BotAI.hpp"
#include "../Utils/UserData/UserData.hpp"
#include "../Utils/Config/ConfigurationManager.hpp"
#include "../Utils/Config/ConfigurationParameters.hpp"
/*** Command lists ***/
#include "./ChatCommands/CustomCommandList.hpp"

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
    // Custom commands
    _chatCommands.push_back(new CustomCommandList());
    // Predefined commands
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

bool BotAI::_CheckIgnoreList(const QString& userName)
{
    return _ignoreList.contains(userName.toLower());
}

///////////////////////////////////////////////////////////////////////////

void BotAI::_UpdateUserData(const ChatMessage& message)
{
    // If user is not in ignore list, update info
    if (!_CheckIgnoreList(message.GetAuthor()))
    {
        _IncrementMessageCounter(message.GetAuthor());
        _AddCurrency(message.GetAuthor(), 1);
    }
}

///////////////////////////////////////////////////////////////////////////

void BotAI::_IncrementMessageCounter(const QString& userName)
{
    UserData& userData = UserData::Instance();
    QString param;
    // Get message counter
    param = userData.GetUserDataParam(userName, UDP_Messages);
    // Increment counter
    param = QString::number(param.toInt() + 1);
    // Set new value
    userData.UpdateUserData(userName, UDP_Messages, param);
}

///////////////////////////////////////////////////////////////////////////

void BotAI::_AddCurrency(const QString& userName, const int value)
{
    UserData& userData = UserData::Instance();
    QString param;
    // Get currency value
    param = userData.GetUserDataParam(userName, UDP_Currency);
    // Update value
    param = QString::number(param.toInt() + value);
    // Set new value
    userData.UpdateUserData(userName, UDP_Currency, param);
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
