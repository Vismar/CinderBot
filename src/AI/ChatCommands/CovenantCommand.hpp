/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
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
    Cov_Disband,
    Cov_Create,
    Cov_Rename,
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
     * Get answer for Cov_Leave command
     * \param message - chat message that will be used for getting needed data
     * \return answer which will notify user about leaving the covenant
     */
    QString _GetAnswerForCovLeaveCmd(const ChatMessage& message);
    /*!
     * Get answer for Cov_Disband command
     * \param message - chat message that will be used for getting needed data
     * \return answer which will notify user about creating covenant
     */
    QString _GetAnswerForCovDisbandCmd(const ChatMessage& message);
    /*!
     * Get answer for Cov_Create command
     * \param message - chat message that will be used for getting needed data
     * \return answer which will notify user about creating covenant
     */
    QString _GetAnswerForCovCreateCmd(const ChatMessage& message);
    /*!
     * Get answer for Cov_Rename command
     * \param message - chat message that will be used for getting needed data
     * \return answer which will notify user about renaming covenant
     */
    QString _GetAnswerForCovRenameCmd(const ChatMessage& message);

    /*** Cov_Leave helper functions ***/
    /*!
     * Return end of name that should start after ampersand
     * \param ampersandPosition - position of ampersand
     * \param message - message where to search name
     * \return position which specifies end of name
     */
    int _GetEndOfNameFromAmpersand(int ampersandPosition, const QString& message);
    /*!
     * Try to set new leader to specified covenant and make old leader just a viewer
     * \param newLeader - name of new leader
     * \param oldLeader - name of old leader
     * \param covName - covenant name
     * \return true if new leader was set, false of specified user is not member of covenant or something faild
     */
    bool _SetNewLeaderToCovenant(const QString& newLeader, const QString& oldLeader, const QString& covName);

    /*! Command type */
    CovCommandType _cmdType;
};

}
