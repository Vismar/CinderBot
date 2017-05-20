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
    /*!
     * Get answer for Cov_List command
     * \return answer which will include all covenants that are exist
     */
    QString _GetAnswerForCovListCmd();
    /*!
     * Get answer for Cov_Join command
     * \param message - chat message that will be used for getting needed data
     * \return answer which will notify user about join or error message
     */
    QString _GetAnswerForCovJoinCmd(const ChatMessage& message);
    /*!
     * Get answer fir Cov_Leave command
     * \param message - chat message that will be used for getting needed data
     * \return answer which will notify user about leaving the covenant
     */
    QString _GetAnswerForCovLeaveCmd(const ChatMessage& message);

    /*! Command type */
    CovCommandType _cmdType;
};

}
