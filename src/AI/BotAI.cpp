#include "BotAI.hpp"
#include "../Utils/UserData/UserData.hpp"
#include "../Utils/Config/ConfigurationManager.hpp"
#include "../Utils/Config/ConfigurationParameters.hpp"
#include "./ChatCommands/BaseFileChatCommand.hpp"
#include "./ChatCommands/UserDataChatCommand.hpp"
#include <QDebug>

BotAI::BotAI(QObject* parent) : QObject(parent)
{
    // Load IgnoreList
    ConfigurationManager& configMng = ConfigurationManager::Instance();
    QString param;
    if (configMng.GetStringParam(CFGS_IGNORE, param))
    {
        _ignoreList = param.split(',');
    }
    if (configMng.GetStringParam(CFGP_LOGIN_NAME, param))
    {
        _ignoreList += param;
    }

    // Load custom commands
    _chatCommands.push_back(new BaseFileChatCommand());
    BaseFileChatCommand* command = static_cast<BaseFileChatCommand*> (_chatCommands.last());
    if (!command->Initialize())
    {
        delete _chatCommands.last();
        _chatCommands.pop_back();
    }

    // Load predefined commands
    _chatCommands.push_back(new UserDataChatCommand());
}

bool BotAI::_CheckIgnoreList(const QString& userName)
{
    return _ignoreList.contains(userName.toLower());
}

void BotAI::_UpdateUserData(const ChatMessage& message)
{
    if (!_CheckIgnoreList(message.GetAuthor()))
    {
        _IncrementMessageCounter(message.GetAuthor());
        _AddCurrency(message.GetAuthor(), 1);
    }
}

void BotAI::_IncrementMessageCounter(const QString& userName)
{
    UserData& userData = UserData::Instance();
    QString param;
    param = userData.GetUserDataParam(userName, UDP_Messages);
    param = QString::number(param.toInt() + 1);
    userData.UpdateUserData(userName, UDP_Messages, param);
}

void BotAI::_AddCurrency(const QString& userName, const int value)
{
    UserData& userData = UserData::Instance();
    QString param;
    param = userData.GetUserDataParam(userName, UDP_Currency);
    param = QString::number(param.toInt() + value);
    userData.UpdateUserData(userName, UDP_Currency, param);
}

void BotAI::ReadNewMessage(ChatMessage message, bool botMessage)
{
    _UpdateUserData(message);
    if (!botMessage)
    {
        QString answer;

        for (int i = 0; i < _chatCommands.size(); ++i)
        {
            if (_chatCommands[i]->GetAnswer(message, answer))
            {
                emit NewBotMessage(answer);
                break;
            }
        }
    }
}
