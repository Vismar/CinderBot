/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QString>
#include <QHash>

#define UD_UPDATE    UserData::Instance().UpdateUserData
#define UD_GET_PARAM UserData::Instance().GetUserDataParam

/*!
 * UserDataParam
 * Types of user data
 */
enum UserDataParam
{
    UDP_Messages = 0,
    UDP_Currency,
    UDP_Covenant,
    UDP_Author,
    UDP_LastTimeVisited,
    UDP_Bits,
    UDP_End
};

/*!
 * Class UserData
 * Singleton. Store user data
 */
class UserData
{
public:
    static UserData &Instance();
    ~UserData();

    /*!
     * Initialize UDP if it exist
     */
    void Initialize();
    /*!
     * Get string value of a parameter
     * \param(IN) userName - name of a user
     * \param(IN) UDP - user data parameter
     * \return string value of UDP
     */
    QString GetUserDataParam(const QString &userName, UserDataParam UDP);
    /*!
     * Update user data. If user do not exist, add him to the hash table with default values of UDP
     * \param(IN) userName - name of a user
     * \param(IN) UDP - user data parameter
     * \param(IN) newValue - new value of UDP
     */
    void UpdateUserData(const QString &userName, UserDataParam UDP, const QString &newValue);

private:
    /*!
     * Get UDP parameter in string format
     * \param(IN) UDP - user data parameter
     * \return string representation of UDP
     */
    QString _GetUDPParam(UserDataParam UDP);
    /*!
     * Add user data to hash table
     * \param(IN) userName - name of a user
     * \param(IN) params - hash table of params of user
     */
    void _AddUserData(const QString &userName, const QHash<QString, QString> &params);
    /*!
     * Reset UDP of specified user
     * \param(IN) userName - name of a user
     */
    void _ResetToDefaultUserData(const QString &userName);
    /*!
     * Initialize default values of UDP
     */
    void _InitializeDefaultUserData();

    /*! Hash table of default parameters of user data */
    QHash<QString, QString> _defaultParams;
};
