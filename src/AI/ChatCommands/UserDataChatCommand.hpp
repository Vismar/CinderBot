#pragma once
#include "BaseChatCommand.hpp"

/*!
 * Class UserDataChatCommand
 * Handles user data related commands
 */
class UserDataChatCommand : public BaseChatCommand
{
public:
    ////////////////////////////////
    /// BaseChatCommand override
    bool GetAnswer(ChatMessage& message, QString& answer);
};
