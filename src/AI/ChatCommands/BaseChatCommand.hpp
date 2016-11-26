#pragma once
#include <QString>
#include "../../Chat/ChatMessage.hpp"

/*!
 * Class BaseChatCommand
 * Base class to all commands for bot
 */
class BaseChatCommand
{
public:
    virtual ~BaseChatCommand() {}
    /*!
     * Trying to execute command if it was founded in message and return result
     * \param(IN) message - chat message
     * \param(OUT) answer - answer of command if it was executed
     * \return true if command was executed
     */
    virtual bool GetAnswer(const ChatMessage& message, QString& answer)
    {
        Q_UNUSED(message);
        Q_UNUSED(answer);
        return false;
    }
};
