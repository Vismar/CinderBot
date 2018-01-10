/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "BotAI.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Twitch/KrakenClient.hpp"
/*** Command lists ***/
#include "AI/ChatCommands/CustomCommands/CustomCommandList.hpp"
#include "AI/ChatCommands/CustomCommands/CustomCovCommandList.hpp"
#include "AI/ChatCommands/UserDataCommands/UserDataCommandList.hpp"
#include "AI/ChatCommands/CovenantCommands/CovenantCommandList.hpp"
#include "AI/ChatCommands/QuoteCommands/QuoteCommandList.hpp"
/*** Timer command lists ***/
#include "AI/TimerCommands/UserData/UserDataTimerCommandList.hpp"
#include "AI/TimerCommands/CovenantData/CovenantDataTimerCommandList.hpp"

using namespace Command;
using namespace UserDataCmd;
using namespace QuoteCmd;
using namespace CustomChatCmd;
using namespace CovenantCmd;
using namespace TimerCommand;
using namespace UserDataTimerCmd;
using namespace CovenantDataTimerCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;
using namespace Twitch;

///////////////////////////////////////////////////////////////////////////

BotAI::BotAI(QObject *parent) : QObject(parent)
{
    // Load IgnoreList
    ConfigurationManager &configMng = ConfigurationManager::Instance();
    QString param;
    // Fill ignore list
    if (configMng.GetStringParam(CfgParam::IgnoreList, param))
    {
        _ignoreList = param.split(',');
    }
    if (configMng.GetStringParam(CfgParam::LoginName, param))
    {
        // If bot name not added to the list, add it
        if (!_ignoreList.contains(param))
        {
            _ignoreList += param;
        }
    } 

    // Load all kind of commands
    LoadCommands();
    LoadTimerCommands();
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
    // If new message is not created by bot, parse it.
    if (!botMessage)
    {
        // If user is not in ignore list, try to find commands and update user info
        if (!(_CheckIgnoreList(message.GetAuthor()) || _CheckIgnoreList(message.GetRealName())))
        {
            ChatAnswer answer;
            if (message.GetType() == PRIVMSG)
            {
                answer.SetType(Twitch_Chat);
            }
            else if (message.GetType() == WHISPER)
            {
                answer.SetType(Twitch_Whisper);
            }
            answer.SetRealName(message.GetRealName());

            // Try to execute command
            for (int i = 0; i < _chatCommands.size(); ++i)
            {
                // If we found a command, emit event with result and break the loop
                if (_chatCommands[i]->TryExecute(message, answer))
                {
                    emit NewBotMessage(answer);
                    break;
                }
            }

            // Update user data
            switch (message.GetType())
            {
            case PRIVMSG:
            case BITS:
                _UpdateUserData(message);
                break;
            default:
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void BotAI::LoadCommands()
{
    // Predefined commands
    _chatCommands.push_back(new UserDataCommandList());
    _chatCommands.push_back(new CovenantCommandList());
    _chatCommands.last()->Initialize();
    _chatCommands.push_back(new QuoteCommandList());
    // Custom commands for all users and covenants
    _chatCommands.push_back(new CustomCommandList());
    _chatCommands.last()->Initialize();
    _chatCommands.push_back(new CustomCovCommandList());
    _chatCommands.last()->Initialize();
}

///////////////////////////////////////////////////////////////////////////

void BotAI::LoadTimerCommands()
{
    // Predefined timer commands
    _timerCommands.push_back(new UserDataTimerCommandList());
    _timerCommands.last()->StartTimerCommands();
    _timerCommands.push_back(new CovenantDataTimerCommandList);
    _timerCommands.last()->StartTimerCommands();
}

///////////////////////////////////////////////////////////////////////////

bool BotAI::_CheckIgnoreList(const QString &userName) const
{
    return _ignoreList.contains(userName.toLower());
}

///////////////////////////////////////////////////////////////////////////

void BotAI::_UpdateUserData(const ChatMessage &message) const
{
    // If user is not in ignore list, update info
    if (!(_CheckIgnoreList(message.GetAuthor()) || _CheckIgnoreList(message.GetRealName())))
    {
        // Grab user params
        UserDataParams userParams = UserDataDBHelper::GetUserParameters(message.GetUserID());

        // If user exist, update data
        if (userParams.UserID != -1)
        {
            // Update params
            userParams.Author = message.GetAuthor();   // Update display name
            userParams.Name = message.GetRealName();   // Update real name
            userParams.Messages++;                     // Add 1 to msg counter

            // Only if stream is on user should be rewarded with currency
            if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
            {
                // Get amount of currency that should be given to user
                QString tempString = "1"; // Default value
                ConfigurationManager::Instance().GetStringParam(CfgParam::CurrencyPerMsg, tempString);
                userParams.Currency += tempString.toInt(); // Update currency
            }

            // Update bits if any
            if (!message.GetBits().isEmpty())
            {
                int bits = message.GetBits().toInt();
                userParams.Bits += bits;

                // Give currency to user
                QString currencyPerBit = "2"; // Default value
                ConfigurationManager::Instance().GetStringParam(CfgParam::CurrencyPerBit, currencyPerBit);
                userParams.Currency += currencyPerBit.toInt() * bits; // Update currency
            }

            // Update results
            UserDataDBHelper::SetUserParameters(userParams);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
