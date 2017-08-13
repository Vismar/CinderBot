/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCommandDBHelper.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Utils::Database;

/*! Static constant array of parameters that could be stored in config manager. */
static const QStringList CustomCmdStrParam =
{
    "Id",
    "Name",
    "Cooldown",
    "ModeratorOnly",
    "Price",
    "Covenant",
    "WorkInWhisper",
    "WorkInChat"
};

#define CUSTOM_COMMAND     "CustomCommand"
#define CUSTOM_COV_COMMAND "CustomCovCommand"

///////////////////////////////////////////////////////////////////////////

CustomCommandDBHelper &CustomCommandDBHelper::Instance()
{
    static CustomCommandDBHelper instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString CustomCommandDBHelper::InititalizeTables()
{
    QString error;
    error = _InititalizeCustomCommandTables(CUSTOM_COMMAND, "");

    if (error == "OK")
    {
        error = _InititalizeCustomCommandTables(CUSTOM_COV_COMMAND, "Covenant_");
    }

    return error;
}

///////////////////////////////////////////////////////////////////////////

QString CustomCommandDBHelper::GetParameter(CmdType cmdType, const QString &cmdName, CustomCmdParameter cmdParam)
{
    QString result;
    QString tableName = _GetTableName(TableType::Commands, cmdType);
    QString parameter = CustomCmdStrParam[static_cast<int>(cmdParam)];

    DB_QUERY_PTR query = DB_SELECT(tableName, parameter, QString("Name='%1'").arg(cmdName));
    if ((query != nullptr) && (query->first()))
    {
        result = query->value(parameter).toString().replace("''", "'");
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandDBHelper::SetParameter(CmdType cmdType, const QString &cmdName, CustomCmdParameter cmdParam, const QString &value)
{
    QString tableName = _GetTableName(TableType::Commands, cmdType);
    QString guardedValue = value;
    guardedValue.replace("'", "''");
    QString paramValue;

    switch (cmdParam)
    {
    case CustomCmdParameter::Id:
    case CustomCmdParameter::ModeratorOnly:
    case CustomCmdParameter::Price:
        paramValue = QString("%1=%2").arg(CustomCmdStrParam[static_cast<int>(cmdParam)]).arg(guardedValue);
        break;
    default:
        paramValue = QString("%1='%2'").arg(CustomCmdStrParam[static_cast<int>(cmdParam)]).arg(guardedValue);
        break;
    }

    DB_UPDATE(tableName, paramValue, QString("Name='%1'").arg(cmdName));

    switch (cmdType)
    {
    case CmdType::StreamerCmd:
        emit CustomCmdParameterChanged(cmdName, cmdParam, value);
        break;
    case CmdType::CovenantCmd:
        emit CustomCovCmdParameterChanged(cmdName, cmdParam, value);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

QVector<int> CustomCommandDBHelper::GetAnswers(CmdType cmdType, const QString &cmdName)
{
    QString tableName = _GetTableName(TableType::Answers, cmdType);
    QVector<int> answerIds;

    DB_QUERY_PTR query = DB_SELECT(tableName, "Id", QString("Name='%1'").arg(cmdName));
    if (query != nullptr)
    {
        while (query->next())
        {
            answerIds.append(query->value("Id").toInt());
        }
    }

    return answerIds;
}

///////////////////////////////////////////////////////////////////////////

QString CustomCommandDBHelper::GetRandomAnswer(CmdType cmdType, const QString &cmdName)
{
    QString tableName = _GetTableName(TableType::Answers, cmdType);
    QString answer;

    DB_QUERY_PTR query = DB_SELECT(tableName, "*",
                                   QString("Answer IN (SELECT Answer FROM %1 "
                                           "WHERE Name='%2' "
                                           "ORDER BY RANDOM() LIMIT 1)").arg(tableName).arg(cmdName));
    if ((query != nullptr) && (query->first()))
    {
        answer = query->value("Answer").toString();
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

QString CustomCommandDBHelper::GetAnswer(CmdType cmdType, int id)
{
    QString tableName = _GetTableName(TableType::Answers, cmdType);
    QString answer;

    DB_QUERY_PTR query = DB_SELECT(tableName, "Answer", QString("Id=%1").arg(id));
    if ((query != nullptr) && (query->first()))
    {
        answer = query->value("Answer").toString().replace("''", "'");
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandDBHelper::AddAnswer(CmdType cmdType, const QString &cmdName, const QString &answer)
{
    QString tableName  = _GetTableName(TableType::Answers, cmdType);
    QString guardedAnswer = answer;
    guardedAnswer.replace("'", "''");

    DB_INSERT(tableName, QString("NULL, Name='%1', Answer='%2'").arg(cmdName).arg(guardedAnswer));

    switch (cmdType)
    {
    case CmdType::StreamerCmd:
        emit CustomCmdAnswerAdded(cmdName);
        break;
    case CmdType::CovenantCmd:
        emit CustomCovCmdAnswerAdded(cmdName);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandDBHelper::EditAnswer(CmdType cmdType, int id, const QString &answer)
{
    QString tableName  = _GetTableName(TableType::Answers, cmdType);
    QString cmdName;
    QString guardedAnswer = answer;
    guardedAnswer.replace("'", "''");

    DB_UPDATE(tableName, QString("Answer='%1'").arg(guardedAnswer), QString("Id=%1").arg(id));

    DB_QUERY_PTR query = DB_SELECT(tableName, "Name", QString("Id=%1").arg(id));
    if ((query != nullptr) && (query->first()))
    {
        cmdName = query->value("Name").toString();
        if (!cmdName.isEmpty())
        {
            switch (cmdType)
            {
            case CmdType::StreamerCmd:
                emit CustomCmdAnswerEdit(cmdName, id);
                break;
            case CmdType::CovenantCmd:
                emit CustomCovCmdAnswerEdit(cmdName, id);
                break;
            default:
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandDBHelper::DeleteAnswer(CmdType cmdType, int id)
{
    QString tableName  = _GetTableName(TableType::Answers, cmdType);
    QString cmdName;

    DB_DELETE(tableName, QString("Id=%1").arg(id));

    DB_QUERY_PTR query = DB_SELECT(tableName, "Name", QString("Id=%1").arg(id));
    if ((query != nullptr) && (query->first()))
    {
        cmdName = query->value("Name").toString();
        if (!cmdName.isEmpty())
        {
            switch (cmdType)
            {
            case CmdType::StreamerCmd:
                emit CustomCmdAnswerDeleted(cmdName, id);
                break;
            case CmdType::CovenantCmd:
                emit CustomCovCmdAnswerDeleted(cmdName, id);
                break;
            default:
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

QString CustomCommandDBHelper::_GetTableName(TableType tableType, CmdType cmdType)
{
    QString tableName;

    switch (tableType)
    {
    case TableType::Commands:
        switch (cmdType)
        {
        case CmdType::StreamerCmd:
            tableName = CUSTOM_COMMAND;
            tableName += "s";
            break;
        case CmdType::CovenantCmd:
            tableName = CUSTOM_COV_COMMAND;
            tableName += "s";
            break;
        default:
            break;
        }
        break;
    case TableType::Answers:
        switch (cmdType)
        {
        case CmdType::StreamerCmd:
            tableName = CUSTOM_COMMAND;
            tableName += "Answers";
            break;
        case CmdType::CovenantCmd:
            tableName = CUSTOM_COV_COMMAND;
            tableName += "Answers";
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return tableName;
}

///////////////////////////////////////////////////////////////////////////

QString CustomCommandDBHelper::_InititalizeCustomCommandTables(const QString &mainTableName, const QString &indexPrefix)
{
    qDebug() << mainTableName;

    QString error = "OK";

    // Main table
    if (!DB_CREATE_TABLE((mainTableName+"s"), "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                              "Name TEXT NOT NULL,"
                                              "Cooldown TEXT NOT NULL,"
                                              "ModeratorOnly INTEGER NOT NULL,"
                                              "Price INTEGER NOT NULL,"
                                              "Covenant TEXT NOT NULL,"
                                              "WorkInWhisper TEXT DEFAULT 'false',"
                                              "WorkInChat    TEXT DEFAULT 'true'"))
    {
        error = "Database error. Check you log file.";
    }

    // Additional table
    if (!DB_CREATE_TABLE((mainTableName+"Answers"), "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                    "Name TEXT NOT NULL,"
                                                    "Answer TEXT NOT NULL"))
    {
        error = "Database error. Check you log file.";
    }

    if (!DB_CREATE_INDEX((mainTableName+"Answers"), (indexPrefix + "Answer_Index"), "Answer"))
    {
        error = "Database error. Check you log file.";
    }

    if (!DB_CREATE_INDEX((mainTableName+"Answers"), (indexPrefix + "Name_Index"), "Name"))
    {
        error = "Database error. Check you log file.";
    }

    return error;
}

///////////////////////////////////////////////////////////////////////////
