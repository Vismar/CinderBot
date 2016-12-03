#pragma once
#include "ChatCommand.hpp"

namespace Command
{

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
class UserDataCommand : public ChatCommand
{
public:
    /*!
     * Set command type
     * \param(IN) cmdType - user data command type
     */
    void SetCommandType(UDCommandType cmdType);
    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
    void Initialize();

private:
    UDCommandType _cmdType;
};

}
