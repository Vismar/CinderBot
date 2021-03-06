/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include <QString>
#include "Chat/ChatMessage.hpp"

using namespace Chat;

/*!
 * Singleton class that stores user list and other stuff for real time things, like statistics and etc.
 */
class RealTimeUserData : public QObject
{
    Q_OBJECT
public:
    explicit RealTimeUserData(QObject* parent = 0);
    virtual ~RealTimeUserData();

    /*!
     * Create static instance and return pointer to it.
     * \return pointer to a static instance
     */
    static RealTimeUserData* Instance();
    /*!
     * Returns reference to the user list
     * \return const reference to the user list
     */
    const QStringList &GetUserList();
    /*!
     * Returns reference to the moderator list
     * \return const reference to the moderator list
     */
    const QStringList &GetModeList();
    /*!
     * Returns max user number during session
     * \return max number of users
     */
    int GetMaxUserNumber();

    /*!
     * Add user to list
     * \param(IN) chatMessage - chat message, that contains author
     */
    void AddUserToList(const ChatMessage &chatMessage);
    /*!
     * Remove user from list
     * \param(IN) chatMessage - chat message, that contains author
     */
    void RemoveUserFromList(const ChatMessage &chatMessage);
    /*!
     * Add moderator to list
     * \param (IN) chatMessage - chat message, that contains author
     */
    void AddModeToList(const ChatMessage &chatMessage);
    /*!
     * Remove moderator to list
     * \param (IN) chatMessage - chat message, that contains author
     */
    void RemoveModeFromList(const ChatMessage &chatMessage);

    /*!
     * Add one message to msg counter
     */
    void IncrementMsgCounter();
    /*!
     * Returns number of messages that was appeared in chat during the stream session
     * \return number of messages
     */
    unsigned long long GetMsgCounter() const;

signals:
    void UserListChanged();
    void ModeListChanged();

private:
    /*! List of users that are in the chat */
    QStringList _userList;
    /*! List of moderators that are in the chat */
    QStringList _modeList;
    /*! Max number of users in chat */
    int _maxUserNumber;
    /*! Message counter for current session */
    unsigned long long _msgCounter;
};
