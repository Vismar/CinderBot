#pragma once
#include <QString>
#include "../../Chat/ChatMessage.hpp"

class BaseChatCommand
{
public:
    virtual ~BaseChatCommand() {}
    virtual bool GetAnswer(ChatMessage& message, QString& answer)
    {
        Q_UNUSED(message);
        Q_UNUSED(answer);
        return false;
    }
};
