/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CovenantCommands/CovenantBaseCommand.hpp"

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{
/*!
 * \brief All covenant related commands.
 */
namespace CovenantCmd
{

/*!
 * \brief Handles user's leaving from covenant.
 */
class LeaveCovenantCommand : public CovenantBaseCommand
{
public:
    LeaveCovenantCommand();

protected:
    /*///////////////////////////////*/
    /* BaseChatCommand overrides     */
    /*///////////////////////////////*/
    /*!
     * \brief Empty function.
     */
    void Initialize() override {}
    /*!
     * \brief Gets specified answer.
     * \param message - chat message.
     * \param answer - reference to variable which will store answer.
     */
    void _GetAnswer(const ChatMessage &message, ChatAnswer &answer) override;
    /*!
     * \brief Empty function.
     */
    void _GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer) override;

private:
    /*!
     * \brief Try to set new leader to specified covenant and make old leader just a viewer
     * \param newLeader - name of new leader
     * \param oldLeaderID - id of old leader
     * \param covName - covenant name
     * \return True if new leader was set, false of specified user is not member of covenant or something failed
     */
    bool _SetNewLeaderToCovenant(const QString &newLeader, int oldLeaderID, const QString &covName) const;
};

}
}
