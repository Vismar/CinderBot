/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UserData.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

UserData &UserData::Instance()
{
    static UserData instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

UserData::~UserData() {}

///////////////////////////////////////////////////////////////////////////

void UserData::Initialize()
{
    _InitializeDefaultUserData();
    DB_CREATE_TABLE("UserData", "Id              INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "Name            TEXT    NOT NULL UNIQUE,"
                                "Author          TEXT,"
                                "Messages        INTEGER NOT NULL,"
                                "Currency        INTEGER NOT NULL,"
                                "Covenant        TEXT    NOT NULL,"
                                "LastTimeVisited TEXT,"
                                "TimeInChat      INTEGER DEFAULT 0,"
                                "Bits            INTEGER DEFAULT 0");
    DB_CREATE_INDEX("UserData", "Covenant_Index", "Covenant");
}

///////////////////////////////////////////////////////////////////////////

QString UserData::GetUserDataParam(const QString &userName, UserDataParam UDP)
{
    QString returnString = "";

    // If user do not exist, add him to the hash table
    std::shared_ptr<QSqlQuery> query = DB_SELECT("UserData", "*", QString("Name = '%1'").arg(userName));
    if (!query->next())
    {
        _AddUserData(userName.toLower(), _defaultParams);
    }

    // Get user data
    std::shared_ptr<QSqlQuery> dataQuery = DB_SELECT("UserData", _GetUDPParam(UDP), QString("Name = '%1'").arg(userName));
    if (dataQuery->first())
    {
        returnString = dataQuery->value(_GetUDPParam(UDP)).toString();
    }

    return returnString;
}

///////////////////////////////////////////////////////////////////////////

void UserData::UpdateUserData(const QString &userName,
                              UserDataParam UDP,
                              const QString &newValue)
{
    // If user do not exist, add him to the hash table
    std::shared_ptr<QSqlQuery> query = DB_SELECT("UserData", "*", QString("Name = '%1'").arg(userName));
    if (!query->next())
    {
        _AddUserData(userName.toLower(), _defaultParams);
    }

    // Update user data
    switch (UDP)
    {
    case UDP_Messages:
    case UDP_Currency:
    case UDP_Bits:
        DB_UPDATE("UserData", QString("%1 = %2").arg(_GetUDPParam(UDP)).arg(newValue),
                              QString("Name = '%1'").arg(userName));
        break;
    default:
        DB_UPDATE("UserData", QString("%1 = '%2'").arg(_GetUDPParam(UDP)).arg(newValue),
                              QString("Name = '%1'").arg(userName));
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

void UserData::_AddUserData(const QString &userName, const QHash<QString, QString> &params)
{
    QString values = "NULL, ':name', ':author', :msg, :cur, ':cov', ':ltv', :inchat, :bits";
    values.replace(":name", userName);
    values.replace(":author", params[_GetUDPParam(UDP_Author)]);
    values.replace(":msg", params[_GetUDPParam(UDP_Messages)]);
    values.replace(":cur", params[_GetUDPParam(UDP_Currency)]);
    values.replace(":cov", params[_GetUDPParam(UDP_Covenant)]);
    values.replace(":ltv", QDateTime::currentDateTime().toString("d-M-yyy h:m:s"));
    values.replace(":inchat", params[_GetUDPParam((UDP_TimeInChat))]);
    values.replace(":bits", params[_GetUDPParam(UDP_Bits)]);
    DB_INSERT("UserData", values);
}

///////////////////////////////////////////////////////////////////////////

void UserData::_ResetToDefaultUserData(const QString &userName)
{
    _AddUserData(userName.toLower(), _defaultParams);
}

///////////////////////////////////////////////////////////////////////////

void UserData::_InitializeDefaultUserData()
{
    /* Create default values of UDP */
    _defaultParams.insert(_GetUDPParam(UDP_Author),     "NoAuthorNameYet");
    _defaultParams.insert(_GetUDPParam(UDP_Messages),   "0");
    _defaultParams.insert(_GetUDPParam(UDP_Currency),   "0");
    _defaultParams.insert(_GetUDPParam(UDP_Covenant),   "Viewer");
    _defaultParams.insert(_GetUDPParam(UDP_TimeInChat), "0");
    _defaultParams.insert(_GetUDPParam(UDP_Bits),       "0");
}

///////////////////////////////////////////////////////////////////////////

QString UserData::_GetUDPParam(UserDataParam UDP)
{
    QString param;
    switch (UDP)
    {
    case UDP_Messages:
        param = "Messages";
        break;
    case UDP_Currency:
        param = "Currency";
        break;
    case UDP_Covenant:
        param = "Covenant";
        break;
    case UDP_Author:
        param = "Author";
        break;
    case UDP_LastTimeVisited:
        param = "LastTimeVisited";
        break;
    case UDP_TimeInChat:
        param = "TimeInChat";
        break;
    case UDP_Bits:
        param = "Bits";
        break;
    default:
        param = "NoParam";
        break;
    }

    return param;
}

///////////////////////////////////////////////////////////////////////////
