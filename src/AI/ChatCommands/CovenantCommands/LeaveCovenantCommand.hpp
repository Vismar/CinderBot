/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <AI/ChatCommands/InbuiltChatCommand.hpp>

namespace Command
{

/*!
 * class LeaveCovenantCommand
 * Handles user's leaving from covenant
 */
class LeaveCovenantCommand : public InbuiltChatCommand
{
public:
    /*!
     * Deafult Constructor
     */
    LeaveCovenantCommand();

protected:
    ////////////////////////////////
    /// BaseChatCommand overrides
    void Initialize();
    void _GetAnswer(const ChatMessage& message, QStringList& answer);
    void _GetRandomAnswer(const ChatMessage& message, QStringList& answer);

private:
    /*** Helper functions ***/
    /*!
     * Return end of name that should start after ampersand
     * \param ampersandPosition - position of ampersand
     * \param message - message where to search name
     * \return position which specifies end of name
     */
    int _GetEndOfNameFromStart(int startOfName, const QString& message);
    /*!
     * Try to set new leader to specified covenant and make old leader just a viewer
     * \param newLeader - name of new leader
     * \param oldLeader - name of old leader
     * \param covName - covenant name
     * \return true if new leader was set, false of specified user is not member of covenant or something faild
     */
    bool _SetNewLeaderToCovenant(const QString& newLeader, const QString& oldLeader, const QString& covName);
};

}
