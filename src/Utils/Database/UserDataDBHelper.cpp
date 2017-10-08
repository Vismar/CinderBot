/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UserDataDBHelper.hpp"
#include "Utils/Logger.hpp"

static QVector<QString> UserDataParamNames = { "UserID", "Name", "Author", "Messages", "Currency",
                                               "Covenant", "LastTimeVisited", "TimeInChat", "Bits", "Subscription" };

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
    Subscription = (query->value("Subscription").toString() == "true") ? true : false;
}

///////////////////////////////////////////////////////////////////////////

QString UserDataParams::ToString() const
{
    // Id-UserId-Name-Author-Messages-Currency-Covenant-LastTimeVisited-TimeInChat-Bits-Subscription
    QString result = QString("NULL, %1, '%2', '%3', %4, %5, '%6', '%7', %8, %9, '%10'")
                            .arg(UserID).arg(Name).arg(Author).arg(Messages).arg(Currency)
                            .arg(Covenant).arg(LastTimeVisited).arg(MinutesInChat).arg(Bits).arg(Subscription ? "true" : "false");

    return result;
}

///////////////////////////////////////////////////////////////////////////
//                          UserDataDBHelper                            //
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

UserDataDBHelper& UserDataDBHelper::Instance()
{
    static UserDataDBHelper instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString UserDataDBHelper::InitializeTables()
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
                                     "Subscription    TEXT    DEFAULT 'false'"))
    {
        result = "User data table was not created.";
    }
    else
    {
        // Create indexes
        if (!DB_CREATE_INDEX("UserData", "Covenant_Index", "Covenant"))
        {
            result = "Indexes were not created for user data table.";
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

    // Try to start transaction
    if (DB_MANAGER.StartTransaction())
    {
        for (int i = 0; i < dataParamsOfUsers.size(); ++i)
        {
            DB_INSERT("UserData", dataParamsOfUsers[i].ToString(), true);
        }

        // End transaction
        added = DB_MANAGER.EndTransaction();
    }

    // If transaction failed, log error
    if (!added)
    {
        LOG(LogError, "", Q_FUNC_INFO, QString("Not able to add %1 user(s) to database.").arg(dataParamsOfUsers.size()));
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
                                  "Covenant='%5', LastTimerVisited='%6', "
                                  "TimeInChat=%7, Bits=%8, "
                                  "Subscription='%9'").arg(userDataParams.Name).arg(userDataParams.Author)
                                                      .arg(userDataParams.Messages).arg(userDataParams.Currency)
                                                      .arg(userDataParams.Covenant).arg(userDataParams.LastTimeVisited)
                                                      .arg(userDataParams.MinutesInChat).arg(userDataParams.Bits)
                                                      .arg(userDataParams.Subscription ? "true" : "false"));
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
    int currency = GetUserParameter(UserDataParameter::Currency, userID).toInt() + amountOfCurrency;
    DB_UPDATE("UserData", QString("Currency=%1").arg(currency), QString("UserID=%1").arg(userID));
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::GiveCurrencyToUser(int amountOfCurrency, const QString &userName)
{
    int currency = GetUserParameter(UserDataParameter::Currency, userName).toInt() + amountOfCurrency;
    DB_UPDATE("UserData", QString("Currency=%1").arg(currency), QString("Name='%1' OR Author='%1'").arg(userName));
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::GiveBitsToUser(int amountOfBits, int userID)
{
    int bits = GetUserParameter(UserDataParameter::Bits, userID).toInt() + amountOfBits;
    DB_UPDATE("UserData", QString("Bits=%1").arg(bits), QString("UserID=%1").arg(userID));
}

///////////////////////////////////////////////////////////////////////////

void UserDataDBHelper::IncrementUserMsgCounter(int numberOfMsg, int userID)
{
    int msg = GetUserParameter(UserDataParameter::Messages, userID).toInt() + numberOfMsg;
    DB_UPDATE("UserData", QString("Messages=%1").arg(msg), QString("UserID=%1").arg(userID));
}

///////////////////////////////////////////////////////////////////////////
