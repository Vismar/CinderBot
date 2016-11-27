#pragma once
#include "BaseChatCommand.hpp"

/*!
 * Class CovenantChatCommand
 * Handle command for displaying covenant list (#cov_list),
 * joining any covenant by a price (#cov_join) and leaving current covenant.
 */
class CovenantChatCommand : public BaseChatCommand
{
public:
    ////////////////////////////////
    /// BaseChatCommand override
    bool GetAnswer(const ChatMessage& message, QString& answer);
};
