#pragma once
#include "BaseChatCommand.hpp"

class UserDataChatCommand : public BaseChatCommand
{
public:
    bool GetAnswer(ChatMessage& message, QString& answer);
};
