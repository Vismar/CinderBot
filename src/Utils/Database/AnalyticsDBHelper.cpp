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
                                     "TotalFollowers   INTEGER NOT NULL DEFAULT 0,"
                                     "TotalSubscribers INTEGER NOT NULL DEFAULT 0"))
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
        DB_QUERY_PTR query = DB_SELECT("Analytics", "TotalFollowers", "Id IN (SELECT MAX(Id) FROM Analytics WHERE RecordType IN ('VOD', 'Live'))");
        if ((query != nullptr) && query->first())
        {
            totalFollowers = query->value("TotalFollowers").toInt();
        }
    }
    // If stream is off
    else
    {
        // Try to get follower number from record for current day
        DB_QUERY_PTR query = DB_SELECT("Analytics", "TotalFollowers", "Id IN (SELECT MAX(Id) FROM Analytics WHERE RecordType='Day')");
        if ((query != nullptr) && query->first())
        {
            totalFollowers = query->value("TotalFollowers").toInt();
        }
    }

    return totalFollowers;
}

///////////////////////////////////////////////////////////////////////////

int AnalyticsDBHelper::GetLastNumberOfSubscribers()
{
    int totalSubs(0);

    // If stream is on
    if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
    {
        // Try to get follower number from last stream record
        DB_QUERY_PTR query = DB_SELECT("Analytics", "TotalSubscribers", "Id IN (SELECT MAX(Id) FROM Analytics WHERE RecordType IN ('VOD', 'Live'))");
        if ((query != nullptr) && query->first())
        {
            totalSubs = query->value("TotalSubscribers").toInt();
        }
    }
    // If stream is off
    else
    {
        // Try to get follower number from last day record
        DB_QUERY_PTR query = DB_SELECT("Analytics", "TotalSubscribers", "Id IN (SELECT MAX(Id) FROM Analytics WHERE RecordType='Day')");
        if ((query != nullptr) && query->first())
        {
            totalSubs = query->value("TotalSubscribers").toInt();
        }
    }

    return totalSubs;
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
    case KrakenParameter::ChannelSubscribers:
        _HandleChangeOfParameterChannelSubscribers(value.toInt());
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
                                    "Id IN (SELECT MAX(Id) FROM Analytics WHERE RecordType IN ('VOD', 'Live'))"))
        {
            LOG(LogError, "", Q_FUNC_INFO, "Last stream record field 'EndOfTheStream' was not updated!");
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void AnalyticsDBHelper::_HandleChangeOfParameterChannelFollowers(int totalFollowers) const
{
    // Start transaction
    if (DB_MANAGER.StartTransaction())
    {
        // If stream is on, we need to update value in last record for stream
        if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
        {
            if (!DB_UPDATE("Analytics", QString("TotalFollowers=%1").arg(totalFollowers),
                                        "Id=(SELECT MAX(Id) FROM Analytics WHERE RecordType IN ('VOD', 'Live'))"))
            {
                LOG(LogError, "", Q_FUNC_INFO, "Last stream record field 'TotalFollowers' was not updated!");
            }
        }

        // Create record if it not exist
        if (!_IsRecordExistForCurrentDay())
        {
            _CreateRecord(AnalyticsRecordType::Day);
        }

        // Try to update certain record from database
        if (!DB_UPDATE("Analytics", QString("TotalFollowers=%1").arg(totalFollowers), 
                                    QString("StartOfTheStream='%1'").arg(QDateTime::currentDateTime().toString("d-M-yyyy"))))
        {
            LOG(LogError, "", Q_FUNC_INFO, "Stream record field 'TotalFollowers' was not updated!");
        }
    }
    // End transaction
    DB_MANAGER.EndTransaction();
}

///////////////////////////////////////////////////////////////////////////

void AnalyticsDBHelper::_HandleChangeOfParameterChannelSubscribers(int totalSubscribers) const
{
    // Start transaction
    if (DB_MANAGER.StartTransaction())
    {
        // If stream is on, we need to update value in last record for stream
        if (KrakenClient::Instance().GetParameter(KrakenParameter::StreamOn).toBool())
        {
            if (!DB_UPDATE("Analytics", QString("TotalSubscribers=%1").arg(totalSubscribers),
                                        "Id=(SELECT MAX(Id) FROM Analytics WHERE RecordType IN ('VOD', 'Live'))"))
            {
                LOG(LogError, "", Q_FUNC_INFO, "Last stream record field 'TotalSubscribers' was not updated!");
            }
        }

        // Create record if it not exist
        if (!_IsRecordExistForCurrentDay())
        {
            _CreateRecord(AnalyticsRecordType::Day);
        }

        // Try to update certain record from database
        if (!DB_UPDATE("Analytics", QString("TotalSubscribers=%1").arg(totalSubscribers),
                                    QString("StartOfTheStream='%1'").arg(QDateTime::currentDateTime().toString("d-M-yyyy"))))
        {
            LOG(LogError, "", Q_FUNC_INFO, "Stream record field 'TotalSubscribers' was not updated!");
        }
    }
    // End transaction
    DB_MANAGER.EndTransaction();
}

///////////////////////////////////////////////////////////////////////////

bool AnalyticsDBHelper::_IsRecordExistForCurrentDay()
{
    bool exist(false);

    // Check if we already have record with current date
    DB_QUERY_PTR query = DB_SELECT("Analytics", "Id", QString("StartOfTheStream='%1'").arg(QDateTime::currentDateTime().toString("d-M-yyyy")));

    // If no record for current day, then we should create it
    if (query != nullptr && query->first())
    {
        exist = true;
    }

    return exist;
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
    if (!DB_INSERT("Analytics", QString("NULL, '%1', '%2', '%3', %4, %5").arg(strRecordType)
                                                                         .arg(dayTime)
                                                                         .arg(dayTime)
                                                                         .arg(GetLastNumberOfFollowers())
                                                                         .arg(GetLastNumberOfSubscribers())))
    {
        LOG(LogError, "", Q_FUNC_INFO, QString("Stream '%1' record cannot be created!").arg(strRecordType));
    }
}

///////////////////////////////////////////////////////////////////////////
