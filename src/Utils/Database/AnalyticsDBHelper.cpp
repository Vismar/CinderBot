/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AnalyticsDBHelper.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Logger.hpp"
#include <QDateTime>

using namespace Utils::Database;
using namespace Twitch;

///////////////////////////////////////////////////////////////////////////

AnalyticsDBHelper::AnalyticsDBHelper(QObject *parent) : QObject(parent)
{
    connect(&KrakenClient::Instance(), &KrakenClient::ParameterChanged,
            this, &AnalyticsDBHelper::_HandleKrakenParameterChange);
}

///////////////////////////////////////////////////////////////////////////

AnalyticsDBHelper& AnalyticsDBHelper::Instance()
{
    static AnalyticsDBHelper instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString AnalyticsDBHelper::InitializeTables()
{
    QString result = "OK";

    // Try to create database table
    if (DB_CREATE_TABLE("Analytics", "Id               INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "RecordType       TEXT    NOT NULL DEFAULT 'Day',"
                                     "StartOfTheStream TEXT    NOT NULL DEFAULT '',"
                                     "EndOfTheStream   TEXT    NOT NULL DEFAULT '',"
                                     "TotalFollowers   INTEGER NOT NULL DEFAULT 0"))
    {
        // Try to create indexes for created database table
        if (!DB_CREATE_INDEX("Analytics", "RecordType_Index", "RecordType"))
        {
            result = "Indexes for 'RecordType' column were not created for analytics table";
        }
    }
    // Error
    else
    {
        result = "Analytics table was not created.";
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

int AnalyticsDBHelper::GetLastNumberOfFollowers()
{
    int totalFollowers(0);

    // If stream is on
    if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
    {
        // Try to get follower number from last record
        DB_QUERY_PTR query = DB_SELECT("Analytics", "TotalFollowers", "Id IN (SELECT Id FROM Analytics ORDER BY Id DESC LIMIT 1)");
        if ((query != nullptr) && query->first())
        {
            totalFollowers = query->value("TotalFollowers").toInt();
        }
    }
    // If stream is off
    else
    {
        // Try to get follower number from record for current day
        DB_QUERY_PTR query = DB_SELECT("Analytics", "TotalFollowers", QString("StartOfTheStream='%1'").arg(QDateTime::currentDateTime().toString("d-M-yyyy")));
        if ((query != nullptr) && query->first())
        {
            totalFollowers = query->value("TotalFollowers").toInt();
        }
    }

    return totalFollowers;
}

///////////////////////////////////////////////////////////////////////////

void AnalyticsDBHelper::_HandleKrakenParameterChange(KrakenParameter param, const QVariant& value) const
{
    switch (param)
    {
    case KrakenParameter::StreamOn:
        _HandleChangeOfParameterStreamOn(value.toBool());
        break;
    case KrakenParameter::ChannelFollowers:
        _HandleChangeOfParameterChannelFollowers(value.toInt());
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

void AnalyticsDBHelper::_HandleChangeOfParameterStreamOn(bool streamOn) const
{
    // If stream is ON, then we need to create record about it
    if (streamOn)
    {
        // Get stream type
        QString streamType = KrakenClient::Instance().GetParameter(KrakenParameter::StreamType).toString();
        // Get stream type and define record type
        AnalyticsRecordType recordType = (streamType == "live") ? AnalyticsRecordType::Live : AnalyticsRecordType::VOD;
        // Create new record for new stream
        _CreateRecord(recordType);
    }
    // If stream went off, we should update EndOfStream field
    else
    {
        if (!DB_UPDATE("Analytics", QString("EndOfTheStream='%1'").arg(QDateTime::currentDateTime().toString("d-M-yyyy h:m:s")),
            "Id IN (SELECT Id FROM Analytics ORDER BY Id DESC LIMIT 1)"))
        {
            LOG(LogError, "", Q_FUNC_INFO, "Last stream record field 'EndOfTheStream' was not updated!");
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void AnalyticsDBHelper::_HandleChangeOfParameterChannelFollowers(int totalFollowers) const
{
    // If stream is on, just update value in last record
    if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
    {
        if (!DB_UPDATE("Analytics", QString("TotalFollowers=%1").arg(totalFollowers), "Id IN (SELECT Id FROM Analytics ORDER BY Id DESC LIMIT 1)"))
        {
            LOG(LogError, "", Q_FUNC_INFO, "Last stream record field 'TotalFollowers' was not updated!");
        }
    }
    // If stream is off and we detected change of followers, try to create new record with type 'Day' or update the last one
    else
    {
        // Check if we already have record with current date
        DB_QUERY_PTR query = DB_SELECT("Analytics", "Id", QString("StartOfTheStream='%1'").arg(QDateTime::currentDateTime().toString("d-M-yyyy")));
        if (query != nullptr && query->first())
        {
            int id = query->value("Id").toInt();

            // Try to update certain record from database
            if (!DB_UPDATE("Analytics", QString("TotalFollowers=%1").arg(totalFollowers), QString("Id=%1").arg(id)))
            {
                LOG(LogError, "", Q_FUNC_INFO, QString("Stream record Id=%1 field 'TotalFollowers' was not updated!").arg(id));
            }
        }
        else
        {
            // If no record for current day, then create it
            _CreateRecord(AnalyticsRecordType::Day);

            // Try to update field in last created record for current day
            if (!DB_UPDATE("Analytics", QString("TotalFollowers=%1").arg(totalFollowers), "Id IN (SELECT Id FROM Analytics ORDER BY Id DESC LIMIT 1)"))
            {
                LOG(LogError, "", Q_FUNC_INFO, "Last stream record field 'TotalFollowers' was not updated!");
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void AnalyticsDBHelper::_CreateRecord(AnalyticsRecordType recordType) const
{
    QString strRecordType;
    QString dayTime;

    // Define what should be used in creation of record
    switch(recordType)
    {
    case AnalyticsRecordType::Day:
        strRecordType = "Day";
        dayTime = QDateTime::currentDateTime().toString("d-M-yyyy");
        break;
    case AnalyticsRecordType::Live:
        strRecordType = "Live";
        dayTime = QDateTime::currentDateTime().toString("d-M-yyyy h:m:s");
        break;
    case AnalyticsRecordType::VOD:
        strRecordType = "VOD";
        dayTime = QDateTime::currentDateTime().toString("d-M-yyyy h:m:s");
        break;
    default:        
        break;
    }

    // Try to create new record
    if (!DB_INSERT("Analytics", QString("NULL, '%1', '%2', '%3', %4").arg(strRecordType).arg(dayTime).arg(dayTime).arg(GetLastNumberOfFollowers())))
    {
        LOG(LogError, "", Q_FUNC_INFO, QString("Stream '%1' record cannot be created!").arg(strRecordType));
    }
}

///////////////////////////////////////////////////////////////////////////
