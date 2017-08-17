/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomChatCommand.hpp"

using namespace Command::CustomChatCmd;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

CustomChatCommand::CustomChatCommand()
{
    _commandTableName = "CustomCommands";
    _commandAnswersTableName = "CustomCommandAnswers";
    _cmdType = CmdType::StreamerCmd;
}

///////////////////////////////////////////////////////////////////////////

CustomChatCommand::~CustomChatCommand() {}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::Initialize()
{
    CmdParams cmdParams = CustomCommandDBHelper::Instance().GetAllParams(_cmdType, _name);
    // Set data to variables
    _cooldown = cmdParams.Cooldown;
    _moderatorOnly = cmdParams.ModeratorOnly;
    _price = cmdParams.Price;
    _covenant = cmdParams.Covenant;
    _workInWhisper = cmdParams.WorkInWhisper;
    _workInChat = cmdParams.WorkInChat;
}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::InitializeByName(const QString &commandName)
{
    _Clear();
    _name = commandName;
    _isRandom = true;
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::OnParameterChanged(const QString &cmdName, CustomCmdParameter cmdParam, const QString &value)
{
    if (cmdName == _name)
    {
        switch (cmdParam)
        {
        case CustomCmdParameter::WorkInWhisper:
            _workInWhisper = ("true" == value);
            break;
        case CustomCmdParameter::WorkInChat:
            _workInChat = ("true" == value);
            break;
        case CustomCmdParameter::Cooldown:
            _cooldown = QTime::fromString(value, "h:m:s");
            break;
        case CustomCmdParameter::ModeratorOnly:
            _moderatorOnly = ("1" == value);
            break;
        case CustomCmdParameter::Price:
            {
                int price = value.toInt();
                if (price < 0)
                {
                    price = 0;
                }
                _price = price;
            }
            break;
        case CustomCmdParameter::Covenant:
            _covenant = value;
            break;
        default:
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////

void CustomChatCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    if (_CheckModerationFlag(message.IsModerator()) &&
        _CheckCooldown() &&
        _CheckCurrency(message.GetRealName()) &&
        _CheckCovenant(message.GetRealName()))
    {
        // Update user currency value
        _TakeDefaultPriceFromUser(message.GetRealName());

        // Get random answer
        QString longAnswer = CustomCommandDBHelper::Instance().GetRandomAnswer(_cmdType, _name);

        // If answer is less than 500 symbols, then everything ok
        if (longAnswer.length() <= 500)
        {
            answer.AddAnswer(longAnswer);
        }
        // If answer is longer than 500 symbols, so it should be sliced
        else
        {
            // Slice an answer to separate messages
            // until every part of an answer will not be less than 500 symbols
            while (longAnswer.length() > 0)
            {
                QString part = longAnswer.left(500);
                // Try to find last space in this part
                int lastSpace = part.lastIndexOf(' ');
                // If in this part no space were found, just use 500 symbols there
                if (lastSpace != -1)
                {
                    answer.AddAnswer(longAnswer.left(lastSpace+1));
                    longAnswer.remove(0, lastSpace+1);
                }
                // If last space were found, cut the message until this index
                else
                {
                    answer.AddAnswer(part);
                    longAnswer.remove(0, 500);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
