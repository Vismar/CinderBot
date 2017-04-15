#pragma once
#include <QObject>
#include <QString>
#include "../../Chat/ChatMessage.hpp"

/*!
 * Singleton class that stores user list and other stuff fpr real time things, like statistics and etc.
 */
class RealTimeUserData : public QObject
{
    Q_OBJECT
public:
    explicit RealTimeUserData(QObject* parent = 0);
    virtual ~RealTimeUserData();

    /*!
     * Create static instance adn return pointer to it.
     * \return pointer to a static instance
     */
    static RealTimeUserData* Instance();
    /*!
     * Returns reference to the user list
     * \return const reference to the user list
     */
    const QStringList& GetUserList();

public slots:
    /*!
     * Add user to list
     * \param(IN) chatMessage - chat message, that contains author
     */
    void AddUserToList(const ChatMessage& chatMessage);
    /*!
     * Remove user from list
     * \param(IN) chatMessage - chat message, that contains author
     */
    void RemoveUserFromList(const ChatMessage& chatMessage);

signals:
    void UserListChanged();

private:
    /*! List of users that are in the chat */
    QStringList _userList;
};
