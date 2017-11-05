/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CovenantDBHelper.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Logger.hpp"

/*! Names for covenant parameters */
static QVector<QString> CovenantParamsNames = { "Name", "Leader", "Description", "CmdSlots", "Level", "Exp", "MaxMembers" };

using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////
//                              CovParams                                //
///////////////////////////////////////////////////////////////////////////

QString CovParams::ToString() const
{
    // Name-Leader-Description-CmdSlots-Level-Exp-MaxMembers
    QString result = QString("NULL, '%1', '%2', '%3', %4, %5, %6, %7")
                     .arg(Name).arg(Leader).arg(Description).arg(CmdSlots).arg(Level).arg(Exp).arg(MaxMembers);

    return result;
}

///////////////////////////////////////////////////////////////////////////

void CovParams::FillDataFromQuery(DB_QUERY_PTR query)
{
    Name = query->value("Name").toString();
    Leader = query->value("Leader").toString();
    Description = query->value("Description").toString();
    CmdSlots = query->value("CmdSlots").toInt();
    Level = query->value("Level").toInt();
    Exp = query->value("Exp").toInt();
    MaxMembers = query->value("MaxMembers").toInt();
}

///////////////////////////////////////////////////////////////////////////
//                          CovenantDBHelper                             //
///////////////////////////////////////////////////////////////////////////

CovenantDBHelper &CovenantDBHelper::Instance()
{
    static CovenantDBHelper instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString CovenantDBHelper::InitializeTables()
{
    QString result = "OK";

    // Create table
    if (!DB_CREATE_TABLE("Covenants", "Id          INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "Name        TEXT NOT NULL UNIQUE,"
                                      "Leader      TEXT NOT NULL,"
                                      "Description TEXT NOT NULL,"
                                      "CmdSlots    INTEGER NOT NULL,"
                                      "Level       INTEGER NOT NULL,"
                                      "Exp         INTEGER NOT NULL,"
                                      "MaxMembers  INTEGER DEFAULT 10"))
    {
        result = "Covenants table was not created.";
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantDBHelper::CreateCovenant(const QString &covenantName, const QString &leaderName)
{
    bool executed(false);

    // Set covenant name and leader name
    CovParams covParams;
    covParams.Name = covenantName;
    covParams.Leader = leaderName;

    // Try to create new record in database
    if (DB_INSERT("Covenants", covParams.ToString()))
    {
        executed = true;
    }
    else
    {
        LOG(LogError, "", Q_FUNC_INFO, QString("Covenant '%1' for user '%2' was not created.").arg(covenantName).arg(leaderName));
    }

    return executed;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantDBHelper::DisbandCovenant(const QString &covenantName)
{
    bool executed(false);

    // Try to delete record from database
    if (DB_DELETE("Covenants", QString("Name='%1'").arg(covenantName)))
    {
        executed = true;
    }
    else
    {
        LOG(LogError, "", Q_FUNC_INFO, QString("Covenant '%1' was not deleted.").arg(covenantName));
    }

    return executed;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantDBHelper::RenameCovenant(const QString &covenantName, const QString &newCovenantName)
{
    bool executed(false);

    // Try to rename covenant
    if (DB_UPDATE("Covenants", QString("Name='%1'").arg(newCovenantName), QString("Name='%1'").arg(covenantName)))
    {
        executed = true;
    }
    else
    {
        LOG(LogError, "", Q_FUNC_INFO, QString("Covenant '%1' was not renamed to '%2'.").arg(covenantName).arg(newCovenantName));
    }

    return executed;
}

///////////////////////////////////////////////////////////////////////////

QVariant CovenantDBHelper::GetParameter(CovenantParameter param, const QString &covenantName)
{
    QVariant paramValue;

    // Try to get requested parameter
    DB_QUERY_PTR query = DB_SELECT("Covenants", CovenantParamsNames[static_cast<int>(param)], QString("Name='%1'").arg(covenantName));
    if (query != nullptr && query->first())
    {
        paramValue = query->value(CovenantParamsNames[static_cast<int>(param)]);
    }

    return paramValue;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantDBHelper::SetParameter(CovenantParameter param, const QString &value, const QString &covenantName)
{
    bool executed(false);

    switch (param)
    {
    // If parameter is text based
    case CovenantParameter::Name:
    case CovenantParameter::Leader:
    case CovenantParameter::Description:
        if (DB_UPDATE("Covenants", QString("%1='%2'").arg(CovenantParamsNames[static_cast<int>(param)]).arg(value),
                                   QString("Name='%1'").arg(covenantName)))
        {
            executed = true;
        }
        else
        {
            LOG(LogError, "", Q_FUNC_INFO, QString("Parameter '%1' was not updated with value '%2' for covenant '%3'.")
                                           .arg(CovenantParamsNames[static_cast<int>(param)]).arg(value).arg(covenantName));
        }
        break;
    // If parameter is no text based
    default:
        if (DB_UPDATE("Covenants", QString("%1=%2").arg(CovenantParamsNames[static_cast<int>(param)]).arg(value),
                                   QString("Name='%1'").arg(covenantName)))
        {
            executed = true;
        }
        else
        {
            LOG(LogError, "", Q_FUNC_INFO, QString("Parameter '%1' was not updated with value '%2' for covenant '%3'.")
                                           .arg(CovenantParamsNames[static_cast<int>(param)]).arg(value).arg(covenantName));
        }
        break;
    }

    return executed;
}

///////////////////////////////////////////////////////////////////////////

CovParams CovenantDBHelper::GetParams(const QString &covenantName)
{
    CovParams covParams;

    // Try to get params fro specified covenant
    DB_QUERY_PTR query = DB_SELECT("Covenants", "*", QString("Name=%1").arg(covenantName));
    if (query != nullptr && query->first())
    {
        covParams.FillDataFromQuery(query);
    }
    
    return covParams;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantDBHelper::SetParams(CovParams params)
{
    bool executed(false);

    // Try to to update covenant params
    if (DB_UPDATE("Covenants", QString("Leader='%1', Description='%2', CmdSlots=%3, Level=%4, Exp=%5, MaxMembers=%6")
                               .arg(params.Leader).arg(params.Description).arg(params.CmdSlots)
                               .arg(params.Level).arg(params.Exp).arg(params.MaxMembers), 
                               QString("Name='%1'").arg(params.Name)))
    {
        executed = true;
    }
    else
    {
        LOG(LogError, "", Q_FUNC_INFO, QString("Parameters was not setted for covenant '%1'").arg(params.Name));
    }

    return executed;
}

///////////////////////////////////////////////////////////////////////////

QStringList CovenantDBHelper::GetCovenants()
{
    QStringList covenantList;

    // Try to get all covenant names
    DB_QUERY_PTR query = DB_SELECT("Covenants", "Name");
    if (query != nullptr)
    {
        // Append names to list
        while (query->next())
        {
            covenantList.append(query->value("Name").toString());
        }
    }

    return covenantList;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantDBHelper::CheckLeadership(int userID)
{
    bool isLeader(false);

    // Get user params
    UserDataParams userParams = UserDataDBHelper::GetUserParameters(userID);
    
    // If leader name of user`s covenant is the same as user`s name, then he is leader
    if (GetParameter(CovenantParameter::Leader, userParams.Covenant).toString() == userParams.Name)
    {
        isLeader = true;
    }

    return isLeader;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantDBHelper::CheckLeadership(const QString& userName)
{
    bool isLeader(false);

    // Get user params
    UserDataParams userParams = UserDataDBHelper::GetUserParameters(userName);

    // If leader name of user`s covenant is the same as user`s name, then he is leader
    if (GetParameter(CovenantParameter::Leader, userParams.Covenant).toString() == userParams.Name)
    {
        isLeader = true;
    }

    return isLeader;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantDBHelper::IsCovenantExist(const QString& covenantName)
{
    bool exist(false);

    // Try to get covenant with specified name
    DB_QUERY_PTR query = DB_SELECT("Covenants", "*", QString("Name='%1'").arg(covenantName));
    // If at least one record was returned, that means that covenant exist
    if (query != nullptr && query->first())
    {
        exist = true;
    }

    return exist;
}

///////////////////////////////////////////////////////////////////////////
