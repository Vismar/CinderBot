#pragma once
#include "ChatCommand.hpp"

namespace Command
{

/*!
 * CovCommandType enum.
 * Simple enum for covenant command types.
 */
enum CovCommandType
{
    Cov_List =0,
    Cov_Join,
    Cov_Leave,
    Cov_End
};

/*!
 * Class CovenantCommand
 * Store covenant command. Can be initialized as any covenant type.
 */
class CovenantCommand : public ChatCommand
{
public:
    /*!
     * Set command type
     * \param(IN) cmdType - covenant command type
     */
    void SetCommandType(CovCommandType cmdType);
    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
    void Initialize();

private:
    CovCommandType _cmdType;
};

}
