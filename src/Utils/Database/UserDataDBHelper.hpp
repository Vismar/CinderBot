/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QVariant>
#include "Utils/Database/DatabaseManager.hpp"

/*!
 * \brief Contains all utility classes.
 */
namespace Utils
{
/*!
 * \brief Contains all database related classes.
 */
namespace Database
{

/////////////////////////////////////////
// How to add new user data parameter
//
// Step 1: Add new parameter to enum UserDataParameter.
// Step 2: Add new variable to struct UserDataParams.
// Step 3: Update function UserDataParams::FillDataFromQuery().
// Step 4: Update function UserDataParams::ToString().
// Step 5: Update static array in UserDataDBHelper.cpp named UserDataParamNames.
// Step 6: Add new parameter to database table via updating function UserDataDBHelper::InitializeTables().
// Step 7: Update function UserDataDBHelper::SetUserParameters().
// Step 8: Check if you need to update functions UserDataDBHelper::UpdateUserParameter().
//         If new parameter is numerical, then add new parameter to cases.

/*!
 * \brief Simple enumeration of user data parameters.
 */
enum class UserDataParameter
{
    UserID,
    Name,
    Author,
    Messages,
    Currency,
    Covenant,
    LastTimeVisited,
    TimeInChat,
    Bits,
    FollowDate
};

/*!
 * \brief Container that stores all user data parameters.
 * 
 * Containers by default stores basic values for each parameter, 
 * but can be filled with actual info by query from database or manually.
 */
struct UserDataParams
{
    /*!
     * \brief Fill data fields from specified database query.
     * \param query - database query, that contains all required fields.
     * 
     * Specified query should contain all requered fields, or it will crash.
     */
    void FillDataFromQuery(DB_QUERY_PTR query);
    /*!
     * \brief Creates special string that contains full list of params, include "NULL" for actual id in database.
     */
    QString ToString() const;

    /*! Id of user. */
    int UserID = -1;
    /*! Real name of user. */
    QString Name = "";
    /*! Display name of user. */
    QString Author = "";
    /*! Message counter. */
    int Messages = 0;
    /*! Amount of currency. */
    int Currency = 0;
    /*! Covenant in which user is. */
    QString Covenant = "Viewer";
    /*! Timestamp when user last time has visited the chat. */
    QString LastTimeVisited = "";
    /*! Number of minutes that were spent in chat by user. */
    int MinutesInChat = 0;
    /*! Amount of bits that was donated by user to broadcaster. */
    int Bits = 0;
    /*! Date when a user has followed the broadcaster. */
    QString FollowDate = "";
};

#define UDHelper UserDataDBHelper::Instance()

/*!
 * \brief Helper class designed to facilitate the work with the database for user data.
 * 
 * Handles creating database tables, provides simple interface to add user or mupltiple users at once,
 * get user paramter(s), update it, also provides functionality to grab/check special info about specific user(s).
 */
class UserDataDBHelper : public QObject
{
    Q_OBJECT
public:
    UserDataDBHelper();
    static UserDataDBHelper &Instance();

    /*** Users and parameters ***/
    /*!
     * \brief Initialiazes database tables.
     * \return String that contains error message if something goes wrong. Otherwise - "OK".
     */
    QString InitializeTables() const;
    /*!
     * \brief Adds user with specified param to database.
     * \param userDataParams - Parameters of user that should be added.
     */
    static void AddUser(const UserDataParams &userDataParams);
    /*!
     * \brief Add multiple users in one transaction.
     * \param dataParamsOfUsers - array of user parameters.
     * \return True if transaction went well. Otherwise - false.
     */
    static bool AddUsers(const QVector<UserDataParams> &dataParamsOfUsers);
    /*!
     * \brief Grabs all user parameters by specified user id.
     * \param userID - id of user.
     * \return Container that contains all user params. If user do not exist, container will contain default values and error user id (-1).
     */
    static UserDataParams GetUserParameters(int userID);
    /*!
     * \brief Grabs all user parameters by specified user name.
     * \param userName - real name or display name of user.
     * \return Container that contains all user params. If user do not exist, container will contain default values and error user id (-1).
     */
    static UserDataParams GetUserParameters(const QString &userName);
    /*!
     * \brief Hrabs specified user parameter by specified user id.
     * \param userDataParam - parameter that should be grabed.
     * \param userID - id of user.
     * \return Variable that contains requested parameter. If user to not exist, variable will be empty.
     */
    static QVariant GetUserParameter(UserDataParameter userDataParam, int userID);
    /*!
     * \brief Hrabs specified user parameter by specified user name.
     * \param userDataParam - parameter that should be grabed.
     * \param userName - real name or display name of user.
     * \return Variable that contains requested parameter. If user to not exist, variable will be empty.
     */
    static QVariant GetUserParameter(UserDataParameter userDataParam, const QString &userName);
    /*!
     * \brief Updates specified parameter for requested user id.
     * \param userDataParam - parameter that should be updated.
     * \param value - new value for specified parameter.
     * \param userID - id of user.
     */
    static void UpdateUserParameter(UserDataParameter userDataParam, const QString &value, int userID);
    /*!
     * \brief Updates specified parameter for requested user name.
     * \param userDataParam - parameter that should be updated.
     * \param value - new value for specified parameter.
     * \param userName - real name or display name of user.
     */
    static void UpdateUserParameter(UserDataParameter userDataParam, const QString &value, const QString &userName);
    /*!
     * \brief Sets all specified parameters to user.
     * \param userDataParams - Parameters of user that should be setted.
     */
    static void SetUserParameters(UserDataParams userDataParams);


    /*** Special functions ***/
    /*!
     * \brief Check is specified user is follower.
     * \param userID - id of user.
     * \return True if such user id exist in database.
     */
    static bool IsUserFollower(int userID);
    /*!
     * \brief Check is specified user is follower.
     * \param userName - real name or display name of user.
     * \return True if such user name or display name exist in database.
     */
    static bool IsUserFollower(const QString &userName);
    /*!
     * \brief Cheks if suer in specified covenant.
     * \param covenant - covenant to check.
     * \param userID - id of user.
     * \return True if specified user in requested covenant. Otherwise - false.
     */
    static bool IsUserInCovenant(const QString &covenant, int userID);
    /*!
     * \brief Cheks if suer in specified covenant.
     * \param covenant - covenant to check.
     * \param userName - real name or display name of user.
     * \return True if specified user in requested covenant. Otherwise - false.
     */
    static bool IsUserInCovenant(const QString &covenant, const QString &userName);
    /*!
     * \brief Grabs real names from specified covenant.
     * \param covenant - covenant from which data will be grabed.
     * \return String list which contains requested data.
     */
    static QStringList GetUsersFromCovenant(const QString &covenant);
    /*!
     * \brief Grabs display names from specified covenant.
     * \param covenant - covenant from which data will be grabed.
     * \return String list which contains requested data.
     */
    static QStringList GetAuthorsFromCovenant(const QString &covenant);
    /*!
     * \brief Grabs user ids from specified covenant.
     * \param covenant - covenant from which data will be grabed.
     * \return String list which contains requested data.
     */
    static QVector<int> GetUserIdsFromCovenant(const QString &covenant);
    /*!
     * \brief Updates covenant field for all users who was in specified covenant.
     * \param currentCovenantName - covenant name that was changed.
     * \param newCovenantName - new covenant name.
     */
    static void UpdateCovenantName(const QString &currentCovenantName, const QString &newCovenantName);
    /*!
     * \brief Gives specified amount of currency to certain user.
     * \param amountOfCurrency - amount of currency that will be given to user. Can be negative.
     * \param userID - id of user.
     */
    static void GiveCurrencyToUser(int amountOfCurrency, int userID);
    /*!
     * \brief Gives specified amount of currency to certain user.
     * \param amountOfCurrency - amount of currency that will be given to user. Can be negative.
     * \param userName - real name or display name of user.
     */
    static void GiveCurrencyToUser(int amountOfCurrency, const QString &userName);
    /*!
     * \brief Gives specified amount of bits to certain user.
     * \param amountOfBits - amount of bits that will be given to user.
     * \param userID - id of user.
     */
    static void GiveBitsToUser(int amountOfBits, int userID);
    /*!
     * \brief Increments number of messages on specified number for certain user.
     * \param numberOfMsg - number of messages that will be added to user.
     * \param userID - id of user.
     */
    static void IncrementUserMsgCounter(int numberOfMsg, int userID);


    /*** RealTimeUserData functions ***/
    /*!
     * \brief Add user to queue which will be used to add users to real time user data table.
     * \param userName - name of user that should be added.
     */
    void AddRealTimeUser(const QString &userName);
    /*!
     * \brief Add user to queue which will be used to delete users from real time user data table.
     * \param userName - name of user that should be added.
     */
    void RemoveRealTimeUser(const QString &userName);
    /*!
     * \brief Add moderator name to moderator list.
     * \param moderatorName - name of moderator.
     */
    void AddModerator(const QString &moderatorName);
    /*!
     * \brief Remove moderator name from moderator list.
     * \param moderatorName - name of moderator.
     */
    void RemoveModerator(const QString &moderatorName);
    /*!
     * \brief Checks if user currently in chat.
     * \param userName - name of user.
     * \return True if specified user found in database. Otherwise - false.
     */
    static bool IsUserInChat(const QString &userName);
    /*!
     * \brief Checks if user is moderator.
     * \param userName - name of user.
     * \return True if specified user in moderator list. Otherwise - false.
     */
    bool IsUserModerator(const QString &userName) const;
    /*!
     * \brief Gives specified amount of currency to all users who is in the chat.
     * \param amountOfCurrency - amount of currency that will be give to all users in chat.
     */
    static void GiveCurrencyToOnlineUsers(int amountOfCurrency);
    /*!
     * \brief Add specified number of minutes to all users who is in the chat.
     * \param minutes - number of minutes that will be added to all users in chat.
     */
    static void UpdateTimeInChat(int minutes);

private slots:
    /*!
     * \brief Add users from _queueIn and remove users from _queueOut to update real time user data table.
     */
    void _UpdateTable();

private:
    /*! Simple timer to update real time user data table. */
    QTimer *_timer;
    /*! Queue which will be used to add users to real time user data table. */
    QQueue<QString> _queueIn;
    /*! Queue which will be used to remove users from real time user data table. */
    QQueue<QString> _queueOut;
    /*! Simple list of moderator who currently in chat. */
    QStringList _moderatorList;
};
    
}
}