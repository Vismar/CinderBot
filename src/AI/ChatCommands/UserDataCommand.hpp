/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/InbuiltChatCommand.hpp"

namespace Command
{

/*!
 * UDCommandType enumeration.
 * Simple enum of UD command types.
 */
enum UDCommandType
{
    UDC_Messages = 0,
    UDC_Currency,
    UDC_Covenant,
    UDC_Full,
    UDC_End
};

/*!
 * Class UserDataCommand
 * Store user data command. Can be inititalized as any user data command type.
 */
class UserDataCommand : public InbuiltChatCommand
{
public:
    /*!
     * Set command type
     * \param(IN) cmdType - user data command type
     */
    void SetCommandType(UDCommandType cmdType);

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage& message, QStringList& answer);
    void _GetRandomAnswer(const ChatMessage& message, QStringList& answer);

private:
    UDCommandType _cmdType;
};

}
