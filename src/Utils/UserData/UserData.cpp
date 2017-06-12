/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "UserData.hpp"
#include "Utils/DatabaseManager.hpp"

///////////////////////////////////////////////////////////////////////////

UserData& UserData::Instance()
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
    DB_CREATE_TABLE("UserData", "Id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "Name     TEXT    NOT NULL UNIQUE,"
                                "Messages INTEGER NOT NULL,"
                                "Currency INTEGER NOT NULL,"
                                "Covenant TEXT    NOT NULL");
    DB_CREATE_INDEX("UserData", "Covenant_Index", "Covenant");
}

///////////////////////////////////////////////////////////////////////////

QString UserData::GetUserDataParam(const QString& userName, UserDataParam UDP)
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

void UserData::UpdateUserData(const QString& userName,
                              UserDataParam UDP,
                              const QString& newValue)
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

void UserData::_AddUserData(const QString& userName, const QHash<QString, QString>& params)
{
    QString values = "NULL, ':name', :msg, :cur, ':cov'";
    values.replace(":name", userName);
    values.replace(":msg", params[_GetUDPParam(UDP_Messages)]);
    values.replace(":cur", params[_GetUDPParam(UDP_Currency)]);
    values.replace(":cov", params[_GetUDPParam(UDP_Covenant)]);
    DB_INSERT("UserData", values);
}

///////////////////////////////////////////////////////////////////////////

void UserData::_ResetToDefaultUserData(const QString& userName)
{
    _AddUserData(userName, _defaultParams);
}

///////////////////////////////////////////////////////////////////////////

void UserData::_InitializeDefaultUserData()
{
    /* Create default values of UDP */
    _defaultParams.insert(_GetUDPParam(UDP_Messages), "0");
    _defaultParams.insert(_GetUDPParam(UDP_Currency), "0");
    _defaultParams.insert(_GetUDPParam(UDP_Covenant), "Viewer");
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
    default:
        param = "NoParam";
        break;
    }

    return param;
}

///////////////////////////////////////////////////////////////////////////
