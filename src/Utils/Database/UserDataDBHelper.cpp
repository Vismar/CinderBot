/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UserDataDBHelper.hpp"
#include "Utils/Logger.hpp"
#include <QQueue>

static QVector<QString> UserDataParamNames = { "UserID", "Name", "Author", "Messages", "Currency",
                                               "Covenant", "LastTimeVisited", "TimeInChat", "Bits",
                                               "FollowDate" };

using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////
//                           UserDataParams                             //
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void UserDataParams::FillDataFromQuery(DB_QUERY_PTR query)
{
    UserID = query->value("UserID").toInt();
    Name = query->value("Name").toString();
    Author = query->value("Author").toString();
    Messages = query->value("Messages").toInt();
    Currency = query->value("Currency").toInt();
    Covenant = query->value("Covenant").toString();
    LastTimeVisited = query->value("LastTimeVisited").toString();
    MinutesInChat = query->value("TimeInChat").toInt();
    Bits = query->value("Bits").toInt();
    FollowDate = query->value("FollowDate").toString();
}

///////////////////////////////////////////////////////////////////////////

QString UserDataParams::ToString() const
{
    // Id-UserId-Name-Author-Messages-Currency-Covenant-LastTimeVisited-TimeInChat-Bits-Subscription
    QString result = QString("NULL, %1, '%2', '%3', %4, %5, '%6', '%7', %8, %9, '%10'")
                            .arg(UserID).arg(Name).arg(Author).arg(Messages).arg(Currency)
                            .arg(Covenant).arg(LastTimeVisited).arg(MinutesInChat).arg(Bits).arg(FollowDate);

    return result;
}

///////////////////////////////////////////////////////////////////////////
//                          UserDataDBHelper                            //
///////////////////////////////////////////////////////////////////////////

UserDataDBHelper::UserDataDBHelper() : QObject(nullptr)
{
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout,
            this, &UserDataDBHelper::_UpdateTable);
}

///////////////////////////////////////////////////////////////////////////

UserDataDBHelper& UserDataDBHelper::Instance()
{
    static UserDataDBHelper instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString UserDataDBHelper::InitializeTables() const
{
    QString result = "OK";

    // Create table
    if (!DB_CREATE_TABLE("UserData", "Id              INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "UserID          INTEGER DEFAULT 0,"
                                     "Name            TEXT    NOT NULL UNIQUE,"
                                     "Author          TEXT    DEFAULT '',"
                                     "Messages        INTEGER NOT NULL,"
                                     "Currency        INTEGER NOT NULL,"
                                     "Covenant        TEXT    NOT NULL,"
                                     "LastTimeVisited TEXT    DEFAULT '',"
                                     "TimeInChat      INTEGER DEFAULT 0,"
                                     "Bits            INTEGER DEFAULT 0,"
                                     "FollowDate      TEXT    DEFAULT ''"))
    {
        result = "User data table was not created.";
    }
    else
    {
        // Create indexes
        if (!DB_CREATE_INDEX("UserData", "Covenant_Index", "Covenant"))
        {
            result = "Indexes for 'Covenant' column were not created for user data table.";
        }
        else if (!DB_CREATE_INDEX("UserData", "UserID_Index", "UserID", true))
        {
            result = "Indexes for 'UserID' column were not created for user data table.";
        }
    }

    if (result == "OK")
    {
        if (DB_CREATE_TABLE("RealTimeUserData", "Id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                  "Name TEXT NOT NULL UNIQUE"))
        {
            // We MUST clear this table at launch to make sure that old data will not affect new session
            DB_DELETE("RealTimeUserData");

            // Every 20 secs we should update table
            _timer->start(20000);
        }
        else
        {
            result = "Real time user data table was no created.";
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::AddUser(const UserDataParams &userDataParams)
{
    DB_INSERT("UserData", userDataParams.ToString(), true);
}

///////////////////////////////////////////////////////////////////////////

bool UserDataDBHelper::AddUsers(const QVector<UserDataParams> &dataParamsOfUsers)
{
    bool added(false);

    // Only if we have at least 1 user to add we should start transaction
    if (dataParamsOfUsers.size() > 0)
    {
        // Try to start transaction
        if (DB_MANAGER.StartTransaction())
        {
            for (int i = 0; i < dataParamsOfUsers.size(); ++i)
            {
                UserDataParams userParams = GetUserParameters(dataParamsOfUsers[i].UserID);

                // If user already in database, then we should only update specific values
                if (userParams.UserID != -1)
                {
                    userParams.Name = dataParamsOfUsers[i].Name;
                    userParams.Author = dataParamsOfUsers[i].Author;
                    userParams.FollowDate = dataParamsOfUsers[i].FollowDate;
                    SetUserParameters(userParams);
                }
                // If user is not in database, then just add a record
                else
                {
                    if (!DB_INSERT("UserData", dataParamsOfUsers[i].ToString(), true))
                    {
                        LOG(LogError, "", Q_FUNC_INFO, QString("User was not added!\nData: %1").arg(dataParamsOfUsers[i].ToString()));
                    }
                }
            }

            // End transaction
            added = DB_MANAGER.EndTransaction();
        }

        // If transaction failed, log error
        if (!added)
        {
            LOG(LogError, "", Q_FUNC_INFO, QString("Not able to add %1 user(s) to database.").arg(dataParamsOfUsers.size()));
        }
    }   

    return added;
}

///////////////////////////////////////////////////////////////////////////

UserDataParams UserDataDBHelper::GetUserParameters(int userID)
{
    UserDataParams userDataParams;

    // Get all data for requested user
    DB_QUERY_PTR query = DB_SELECT("UserData", "*", QString("UserID=%1").arg(userID));
    if (query != nullptr && query->first())
    {
        // Fill struct if query returns data
        userDataParams.FillDataFromQuery(query);
    }

    return userDataParams;
}

///////////////////////////////////////////////////////////////////////////

UserDataParams UserDataDBHelper::GetUserParameters(const QString &userName)
{
    UserDataParams userDataParams;

    // Get all data for requested user
    DB_QUERY_PTR query = DB_SELECT("UserData", "*", QString("Name='%1' OR Author='%1'").arg(userName));
    if (query != nullptr && query->first())
    {
        // Fill struct if query returns data
        userDataParams.FillDataFromQuery(query);
    }

    return userDataParams;
}

///////////////////////////////////////////////////////////////////////////

QVariant UserDataDBHelper::GetUserParameter(UserDataParameter userDataParam, int userID)
{
    QVariant param;

    // Get requested parameter for specified user
    DB_QUERY_PTR query = DB_SELECT("UserData", UserDataParamNames[static_cast<int>(userDataParam)], QString("UserID=%1").arg(userID));
    if (query != nullptr && query->first())
    {
        // Set param value
        param = query->value(UserDataParamNames[static_cast<int>(userDataParam)]);
    }

    return param;
}

///////////////////////////////////////////////////////////////////////////

QVariant UserDataDBHelper::GetUserParameter(UserDataParameter userDataParam, const QString &userName)
{
    QVariant param;

    // Get requested parameter for specified user
    DB_QUERY_PTR query = DB_SELECT("UserData", UserDataParamNames[static_cast<int>(userDataParam)], QString("Name='%1' OR Author='%1'").arg(userName));
    if (query != nullptr && query->first())
    {
        // Set param value
        param = query->value(UserDataParamNames[static_cast<int>(userDataParam)]);
    }

    return param;
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::UpdateUserParameter(UserDataParameter userDataParam, const QString &value, int userID)
{
    // Only if user exist in database, update parameter
    if (IsUserFollower(userID))
    {
        switch (userDataParam)
        {
        // For numerical data types we should user template "param = value"
        case UserDataParameter::UserID:
        case UserDataParameter::Messages:
        case UserDataParameter::Currency:
        case UserDataParameter::TimeInChat:
        case UserDataParameter::Bits:
            DB_UPDATE("UserData", QString("%1=%2").arg(UserDataParamNames[static_cast<int>(userDataParam)]).arg(value),
                                  QString("UserID=%1").arg(userID));
            break;
        // For text data types we should user template "param = 'value'"
        default:
            DB_UPDATE("UserData", QString("%1='%2'").arg(UserDataParamNames[static_cast<int>(userDataParam)]).arg(value),
                                  QString("UserID=%1").arg(userID));
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::UpdateUserParameter(UserDataParameter userDataParam, const QString &value, const QString &userName)
{
    if (IsUserFollower(userName))
    {
        switch (userDataParam)
        {
        // For numerical data types we should user template "param = value"
        case UserDataParameter::UserID:
        case UserDataParameter::Messages:
        case UserDataParameter::Currency:
        case UserDataParameter::TimeInChat:
        case UserDataParameter::Bits:
            DB_UPDATE("UserData", QString("%1=%2").arg(UserDataParamNames[static_cast<int>(userDataParam)]).arg(value),
                                  QString("Name='%1' OR Author='%1'").arg(userName));
            break;
        // For text data types we should user template "param = 'value'"
        default:
            DB_UPDATE("UserData", QString("%1='%2'").arg(UserDataParamNames[static_cast<int>(userDataParam)]).arg(value),
                                  QString("Name='%1' OR Author='%1'").arg(userName));
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::SetUserParameters(UserDataParams userDataParams)
{
    DB_UPDATE("UserData", QString("Name='%1', Author='%2', "
                                  "Messages=%3, Currency=%4, "
                                  "Covenant='%5', LastTimeVisited='%6', "
                                  "TimeInChat=%7, Bits=%8, "
                                  "FollowDate='%10'")
                                  .arg(userDataParams.Name).arg(userDataParams.Author)
                                  .arg(userDataParams.Messages).arg(userDataParams.Currency)
                                  .arg(userDataParams.Covenant).arg(userDataParams.LastTimeVisited)
                                  .arg(userDataParams.MinutesInChat).arg(userDataParams.Bits)
                                  .arg(userDataParams.FollowDate),
                          QString("UserID=%1").arg(userDataParams.UserID));
}

///////////////////////////////////////////////////////////////////////////

bool UserDataDBHelper::IsUserFollower(int userID)
{
    bool exist(false);

    // Try to get user from database
    DB_QUERY_PTR query = DB_SELECT("UserData", "*", QString("UserID=%1").arg(userID));
    if (query != nullptr && query->first())
    {
        // If we have returned record, that means user exist in database
        exist = true;
    }

    return exist;
}

///////////////////////////////////////////////////////////////////////////

bool UserDataDBHelper::IsUserFollower(const QString &userName)
{
    bool exist(false);

    // Try to get user from database
    DB_QUERY_PTR query = DB_SELECT("UserData", "*", QString("Name='%1' OR Author='%1'").arg(userName));
    if (query != nullptr && query->first())
    {
        // If we have returned record, that means user exist in database
        exist = true;
    }

    return exist;
}

///////////////////////////////////////////////////////////////////////////

bool UserDataDBHelper::IsUserInCovenant(const QString& covenant, int userID)
{
    bool inCovenant(false);

    DB_QUERY_PTR query = DB_SELECT("UserData", "Covenant", QString("UserID=%1").arg(userID));
    if (query != nullptr && query->first())
    {
        // If user in covenant, set returning value to true
        if (query->value("Covenant").toString() == covenant)
        {
            inCovenant = true;
        }
    }

    return inCovenant;
}

///////////////////////////////////////////////////////////////////////////

bool UserDataDBHelper::IsUserInCovenant(const QString& covenant, const QString& userName)
{
    bool inCovenant(false);

    DB_QUERY_PTR query = DB_SELECT("UserData", "Covenant", QString("Name='%1' OR Author='%1'").arg(userName));
    if (query != nullptr && query->first())
    {
        // If user in covenant, set returning value to true
        if (query->value("Covenant").toString() == covenant)
        {
            inCovenant = true;
        }
    }

    return inCovenant;
}

///////////////////////////////////////////////////////////////////////////

QStringList UserDataDBHelper::GetUsersFromCovenant(const QString &covenant)
{
    QStringList users;

    // Grab all real names from database where covenant field is equal to requested
    DB_QUERY_PTR query = DB_SELECT("UserData", "Name", QString("Covenant='%1'").arg(covenant));
    if (query != nullptr)
    {
        while (query->next())
        {
            users.append(query->value("Name").toString());
        }
    }

    return users;
}

///////////////////////////////////////////////////////////////////////////

QStringList UserDataDBHelper::GetAuthorsFromCovenant(const QString &covenant)
{
    QStringList users;

    // Grab all author names from database where covenant field is equal to requested
    DB_QUERY_PTR query = DB_SELECT("UserData", "Author", QString("Covenant='%1'").arg(covenant));
    if (query != nullptr)
    {
        while (query->next())
        {
            users.append(query->value("Author").toString());
        }
    }

    return users;
}

///////////////////////////////////////////////////////////////////////////

QVector<int> UserDataDBHelper::GetUserIdsFromCovenant(const QString &covenant)
{
    QVector<int> userIDs;

    // Grab all user ids from database where covenant field is equal to requested
    DB_QUERY_PTR query = DB_SELECT("UserData", "UserID", QString("Covenant='%1'").arg(covenant));
    if (query != nullptr)
    {
        while (query->next())
        {
            userIDs.push_back(query->value("UserID").toInt());
        }
    }

    return userIDs;
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::UpdateCovenantName(const QString& currentCovenantName, const QString& newCovenantName)
{
    DB_UPDATE("UserData", QString("Covenant='%1'").arg(newCovenantName), QString("Covenant='%1'").arg(currentCovenantName));
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::GiveCurrencyToUser(int amountOfCurrency, int userID)
{
    DB_UPDATE("UserData", QString("Currency=Currency+%1").arg(amountOfCurrency), QString("UserID=%1").arg(userID));
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::GiveCurrencyToUser(int amountOfCurrency, const QString &userName)
{
    DB_UPDATE("UserData", QString("Currency=Currency+%1").arg(amountOfCurrency), QString("Name='%1' OR Author='%1'").arg(userName));
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::GiveBitsToUser(int amountOfBits, int userID)
{
    DB_UPDATE("UserData", QString("Bits=Bits+%1").arg(amountOfBits), QString("UserID=%1").arg(userID));
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::IncrementUserMsgCounter(int numberOfMsg, int userID)
{
    DB_UPDATE("UserData", QString("Messages=Messages+%1").arg(numberOfMsg), QString("UserID=%1").arg(userID));
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::AddRealTimeUser(const QString& userName)
{
    _queueIn.push_back(userName);
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::RemoveRealTimeUser(const QString& userName)
{
    _queueOut.push_back(userName);
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::_UpdateTable()
{
    // If queues are not empty, then we should to update table
    if (!_queueIn.isEmpty() || !_queueOut.isEmpty())
    {
        // Start transaction
        if (DB_MANAGER.StartTransaction())
        {
            // Add every user to table
            while (!_queueIn.isEmpty())
            {
                if (!DB_INSERT("RealTimeUserData", QString("NULL, '%1'").arg(_queueIn.dequeue()), true))
                {
                    LOG(LogError, "", Q_FUNC_INFO, "User was not added to real time user data table.");
                }
            }

            // Get all user names in 1 string to delete in one request
            QString fullNameList;
            while (!_queueOut.isEmpty())
            {
                // Between names we should put a comma
                if (!fullNameList.isEmpty())
                {
                    fullNameList.append(",");
                }

                // Every name should be in single quotes
                fullNameList.append("'" + _queueOut.dequeue() + "'");
            }

            // If we have at least one user to delete, delete
            if (!fullNameList.isEmpty())
            {
                if (!DB_DELETE("RealTimeUserData", QString("Name IN (%1)").arg(fullNameList)))
                {
                    LOG(LogError, "", Q_FUNC_INFO, QString("Users (%1) was not deleted.").arg(fullNameList));
                }
            }

            // End transaction
            DB_MANAGER.EndTransaction();
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::AddModerator(const QString& moderatorName)
{
    if (!_moderatorList.contains(moderatorName))
    {
        _moderatorList.append(moderatorName);
    }
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::RemoveModerator(const QString& moderatorName)
{
    _moderatorList.removeOne(moderatorName);
}

///////////////////////////////////////////////////////////////////////////

bool UserDataDBHelper::IsUserInChat(const QString& userName)
{
    bool inChat(false);

    // Check if specified user name is in database
    DB_QUERY_PTR query = DB_SELECT("RealTimeUserData", "*", QString("Name='%1'").arg(userName));
    if (query != nullptr && query->first())
    {
        inChat = true;
    }

    return inChat;
}

///////////////////////////////////////////////////////////////////////////

bool UserDataDBHelper::IsUserModerator(const QString& userName) const
{
    return _moderatorList.contains(userName);
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::GiveCurrencyToOnlineUsers(int amountOfCurrency)
{
    DB_UPDATE("UserData", QString("Currency=Currency+%1").arg(amountOfCurrency), "Name IN (SELECT Name FROM RealTimeUserData)");
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::UpdateTimeInChat(int minutes)
{
    DB_UPDATE("UserData", QString("TimeInChat=TimeInChat+%1").arg(minutes), "Name IN (SELECT Name FROM RealTimeUserData)");
}

///////////////////////////////////////////////////////////////////////////