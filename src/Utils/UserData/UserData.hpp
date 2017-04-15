#pragma once
#include <QString>
#include <QHash>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

/*!
 * UserDataParam
 * Types of user data
 */
enum UserDataParam
{
    UDP_Messages = 0,
    UDP_Currency,
    UDP_Covenant,
    UDP_End
};

/*!
 * Class UserData
 * Singleton. Store user data
 */
class UserData
{
public:
    static UserData& Instance();
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
    QString GetUserDataParam(const QString& userName, UserDataParam UDP);
    /*!
     * Update user data. If user do not exist, add him to the hash table with default values of UDP
     * \param(IN) userName - name of a user
     * \param(IN) UDP - user data parameter
     * \param(IN) newValue - new value of UDP
     */
    void UpdateUserData(const QString& userName, UserDataParam UDP, const QString& newValue);

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
    void _AddUserData(const QString& userName, const QHash<QString, QString>& params);
    /*!
     * Reset UDP of specified user
     * \param(IN) userName - name of a user
     */
    void _ResetToDefaultUserData(const QString& userName);
    /*!
     * Initialize default values of UDP
     */
    void _InitializeDefaultUserData();

    /*!
     * Read xml file to get UDP
     * \return hash table of UDP
     */
    QHash<QString, QString> _ReadUserParams();
    /*!
     * Read user data from xml file
     */
    void _ReadUserData();
    /*!
     * Save stored user data to file
     */
    void _SaveUserData();

    /*! Hash table of user data */
    QHash<QString, QHash<QString, QString> > _userData;
    /*! Hash table if default parameters of user data */
    QHash<QString, QString>                  _defaultParams;
    /*! Xml stream reader */
    QXmlStreamReader _xmlReader;
    /*! Xml stream writer */
    QXmlStreamWriter _xmlWriter;
};
