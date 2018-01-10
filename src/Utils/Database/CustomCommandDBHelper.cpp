/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CustomCommandDBHelper.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Utils::Database;

#define CUSTOM_COMMAND     "CustomCommand"
#define CUSTOM_COV_COMMAND "CustomCovCommand"

///////////////////////////////////////////////////////////////////////////
//                            Additional things                          //
///////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////

/*! 
 * \brief Utility function to fill CmdParams from query
 */
void SetParamsFromQuery(DB_QUERY_PTR query, CmdParams &cmdParams)
{
    if ((query != nullptr) && query->first())
    {
        cmdParams.Cooldown = QTime::fromString(query->value("Cooldown").toString(), "h:m:s");
        cmdParams.ModeratorOnly = query->value("ModeratorOnly").toBool();
        cmdParams.Price = query->value("Price").toInt();
        if (cmdParams.Price < 0)
        {
            cmdParams.Price = 0;
        }
        cmdParams.Covenant = query->value("Covenant").toString();
        cmdParams.WorkInWhisper = ("true" == query->value("WorkInWhisper").toString());
        cmdParams.WorkInChat = ("true" == query->value("WorkInChat").toString());
    }
}

///////////////////////////////////////////////////////////////////////////
//                               CmdParams                               //
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

QString CmdParams::ToParamString() const
{
    QString params = "Cooldown=':cooldown', "
                     "ModeratorOnly=:mod, "
                     "Price=:price, "
                     "Covenant=':cov', "
                     "WorkInWhisper=':whisper', "
                     "WorkInChat=':chat'";
    params.replace(":cooldown", Cooldown.toString("h:m:s"));
    params.replace(":mod", (ModeratorOnly ? "1" : "0"));
    params.replace(":price", QString::number(Price));
    params.replace(":cov", Covenant);
    params.replace(":whisper", (WorkInWhisper ? "true" : "false"));
    params.replace(":chat", (WorkInChat ? "true" : "false"));

    return params;
}

///////////////////////////////////////////////////////////////////////////

QString CmdParams::ToAddString() const
{
    QString params = "':cooldown', "
                     ":mod, "
                     ":price, "
                     "':cov', "
                     "':whisper', "
                     "':chat'";
    params.replace(":cooldown", Cooldown.toString("h:m:s"));
    params.replace(":mod", (ModeratorOnly ? "1" : "0"));
    params.replace(":price", QString::number(Price));
    params.replace(":cov", Covenant);
    params.replace(":whisper", (WorkInWhisper ? "true" : "false"));
    params.replace(":chat", (WorkInChat ? "true" : "false"));

    return params;
}

///////////////////////////////////////////////////////////////////////////

void CmdParams::GuardParams()
{
    if (Price < 0)
    {
        Price = 0;
    }
}

///////////////////////////////////////////////////////////////////////////
//                        CustomCommandDBHelper                          //
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

CustomCommandDBHelper &CustomCommandDBHelper::Instance()
{
    static CustomCommandDBHelper instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString CustomCommandDBHelper::InititalizeTables()
{
    // Initialize tables for custom commands
    QString error = _InititalizeCustomCommandTables(CUSTOM_COMMAND, "");

    // If everything is okay, initialize tables for custom covenant commands
    if (error == "OK")
    {
        error = _InititalizeCustomCommandTables(CUSTOM_COV_COMMAND, "Covenant_");
    }

    return error;
}

///////////////////////////////////////////////////////////////////////////

bool CustomCommandDBHelper::CommandExist(const QString &cmdName, const QString &covenant)
{
    bool found(false);

    // Check if command exist in user command list
    QString tableName = _GetTableName(TableType::Commands, CmdType::StreamerCmd);
    DB_QUERY_PTR customCmdQuery;
    if (covenant.isEmpty())
    {
        customCmdQuery = DB_SELECT(tableName, "Id", QString("Name='%1'").arg(cmdName));
    }
    else
    {
        customCmdQuery = DB_SELECT(tableName, "Id", QString("Name='%1' AND Covenant='%2'").arg(cmdName).arg(covenant));
    }
    if ((customCmdQuery != nullptr) && (customCmdQuery->first()))
    {
        found = true;
    }

    // Check if command exist in cov command list
    tableName = _GetTableName(TableType::Commands, CmdType::CovenantCmd);
    DB_QUERY_PTR customCovCmdQuery;
    if (covenant.isEmpty())
    {
        customCovCmdQuery = DB_SELECT(tableName, "Id", QString("Name='%1'").arg(cmdName));
    }
    else
    {
        customCovCmdQuery = DB_SELECT(tableName, "Id", QString("Name='%1' AND Covenant='%2'").arg(cmdName).arg(covenant));
    }
    if ((customCovCmdQuery != nullptr) && (customCovCmdQuery->first()))
    {
        found = true;
    }

    return found;
}

///////////////////////////////////////////////////////////////////////////

bool CustomCommandDBHelper::CreateCommand(CmdType cmdType, const QString &cmdName, CmdParams &cmdParams)
{
    bool created(false);
    cmdParams.GuardParams();

    // Guard " ' " by " '' "
    QString guardedCmdName = cmdName.toLower();
    guardedCmdName.replace("'", "");

    // If command not exist, try to create it
    if (!CommandExist(guardedCmdName))
    {
        // Get params
        QString values = "NULL, 'name', PARAMS";
        values.replace("name", guardedCmdName);
        values.replace("PARAMS", cmdParams.ToAddString());

        // Get table name
        QString tableName = _GetTableName(TableType::Commands, cmdType);
        if (DB_INSERT(tableName, values))
        {
            // Check if command was created and get id of it
            DB_QUERY_PTR query = DB_SELECT(tableName, "Id", QString("Name='%1'").arg(guardedCmdName));
            if ((query != nullptr) && (query->first()))
            {
                created = true;
                emit CustomCmdAdded(cmdType, guardedCmdName, query->value("Id").toInt());
            }
        }
    }

    return created;
}

///////////////////////////////////////////////////////////////////////////

bool CustomCommandDBHelper::DeleteCommand(CmdType cmdType, const QString &cmdName)
{
    bool deleted(false);

    // Check if command exist
    if (CommandExist(cmdName))
    {
        // Get table names
        QString tableNameAnswers = _GetTableName(TableType::Answers, cmdType);
        QString tableNameCommands = _GetTableName(TableType::Commands, cmdType);

        // Get id of command to sent it via signal
        DB_QUERY_PTR query = DB_SELECT(tableNameCommands, "Id", QString("Name='%1'").arg(cmdName));
        if ((query != nullptr) && (query->first()))
        {
            // Delete command from command table and all answers from answers table
            if (DB_DELETE(tableNameAnswers, QString("Name='%1'").arg(cmdName)) &&
                DB_DELETE(tableNameCommands, QString("Name='%1'").arg(cmdName)))
            {
                deleted = true;
                emit CustomCmdDeleted(cmdType, cmdName, query->value("Id").toInt());
            }
        }
    }

    return deleted;
}

///////////////////////////////////////////////////////////////////////////

QVector<int> CustomCommandDBHelper::GetCommandIds(CmdType cmdType, const QString &covenant)
{
    QVector<int> commandIds;
    QString tableName = _GetTableName(TableType::Commands, cmdType);
    QString guardedCovenant = covenant;
    guardedCovenant.replace("'", "''");
    DB_QUERY_PTR query;
    if (guardedCovenant.isEmpty())
    {
        query = DB_SELECT(tableName, "Id");
    }
    else
    {
        query = DB_SELECT(tableName, "Id", QString("Covenant='%1'").arg(guardedCovenant));
    }
    if (query != nullptr)
    {
        while (query->next())
        {
            commandIds.append(query->value("Id").toInt());
        }
    }

    return commandIds;
}

///////////////////////////////////////////////////////////////////////////

QStringList CustomCommandDBHelper::GetCommandNames(CmdType cmdType, const QString &covenant)
{
    QStringList commandNames;
    QString tableName = _GetTableName(TableType::Commands, cmdType);
    QString guardedCovenant = covenant;
    guardedCovenant.replace("'", "''");
    DB_QUERY_PTR query;
    if (guardedCovenant.isEmpty())
    {
        query = DB_SELECT(tableName, "Name");
    }
    else
    {
        query = DB_SELECT(tableName, "Name", QString("Covenant='%1'").arg(guardedCovenant));
    }
    if (query != nullptr)
    {
        while (query->next())
        {
            commandNames.append(query->value("Name").toString());
        }
    }

    return commandNames;
}

///////////////////////////////////////////////////////////////////////////

int CustomCommandDBHelper::GetNumberOfCommands(CmdType cmdType, const QString& covenant)
{
    int numberOfCommands(0);
    QString tableName = _GetTableName(TableType::Commands, cmdType);
    QString guardedCovenant = covenant;
    guardedCovenant.replace("'", "''");
    DB_QUERY_PTR query;
    if (guardedCovenant.isEmpty())
    {
        query = DB_SELECT(tableName, "COUNT(*)");
    }
    else
    {
        query = DB_SELECT(tableName, "COUNT(*)", QString("Covenant='%1'").arg(covenant));
    }
    if ((query != nullptr) && (query->first()))
    {
        numberOfCommands = query->value(0).toInt();
    }

    return numberOfCommands;
}

///////////////////////////////////////////////////////////////////////////

CmdParams CustomCommandDBHelper::GetAllParams(CmdType cmdType, int id)
{
    CmdParams cmdParams;
    QString tableName = _GetTableName(TableType::Commands, cmdType);

    DB_QUERY_PTR query = DB_SELECT(tableName, "*", QString("Id=%1").arg(id));
    SetParamsFromQuery(query, cmdParams);

    return cmdParams;
}

///////////////////////////////////////////////////////////////////////////

CmdParams CustomCommandDBHelper::GetAllParams(CmdType cmdType, const QString &cmdName)
{
    CmdParams cmdParams;
    QString tableName = _GetTableName(TableType::Commands, cmdType);

    DB_QUERY_PTR query = DB_SELECT(tableName, "*", QString("Name='%1'").arg(cmdName));
    SetParamsFromQuery(query, cmdParams);

    return cmdParams;
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandDBHelper::SetAllParams(CmdType cmdType, const QString &cmdName, CmdParams &cmdParams)
{
    QString tableName = _GetTableName(TableType::Commands, cmdType);
    cmdParams.GuardParams();

    if (DB_UPDATE(tableName, cmdParams.ToParamString(), QString("Name='%1'").arg(cmdName)))
    {
        switch (cmdType)
        {
        case CmdType::StreamerCmd:
            emit CustomCmdParameterChanged(cmdName, CustomCmdParameter::Cooldown, cmdParams.Cooldown.toString("h:m:s"));
            emit CustomCmdParameterChanged(cmdName, CustomCmdParameter::ModeratorOnly, (cmdParams.ModeratorOnly ? "1" : "0"));
            emit CustomCmdParameterChanged(cmdName, CustomCmdParameter::Price, QString::number(cmdParams.Price));
            emit CustomCmdParameterChanged(cmdName, CustomCmdParameter::Covenant, cmdParams.Covenant);
            emit CustomCmdParameterChanged(cmdName, CustomCmdParameter::WorkInWhisper, (cmdParams.WorkInWhisper ? "true" : "false"));
            emit CustomCmdParameterChanged(cmdName, CustomCmdParameter::WorkInChat, (cmdParams.WorkInChat ? "true" : "false"));
            break;
        case CmdType::CovenantCmd:
            emit CustomCovCmdParameterChanged(cmdName, CustomCmdParameter::Cooldown, cmdParams.Cooldown.toString("h:m:s"));
            emit CustomCovCmdParameterChanged(cmdName, CustomCmdParameter::ModeratorOnly, (cmdParams.ModeratorOnly ? "1" : "0"));
            emit CustomCovCmdParameterChanged(cmdName, CustomCmdParameter::Price, QString::number(cmdParams.Price));
            emit CustomCovCmdParameterChanged(cmdName, CustomCmdParameter::Covenant, cmdParams.Covenant);
            emit CustomCovCmdParameterChanged(cmdName, CustomCmdParameter::WorkInWhisper, (cmdParams.WorkInWhisper ? "true" : "false"));
            emit CustomCovCmdParameterChanged(cmdName, CustomCmdParameter::WorkInChat, (cmdParams.WorkInChat ? "true" : "false"));
            break;
        default:
            break;
        }
    }
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

QString CustomCommandDBHelper::GetParameter(CmdType cmdType, int id, CustomCmdParameter cmdParam)
{
    QString result;
    QString tableName = _GetTableName(TableType::Commands, cmdType);
    QString parameter = CustomCmdStrParam[static_cast<int>(cmdParam)];

    DB_QUERY_PTR query = DB_SELECT(tableName, parameter, QString("Id=%1").arg(id));
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

    if (DB_UPDATE(tableName, paramValue, QString("Name='%1'").arg(cmdName)))
    {
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
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandDBHelper::UpdateCovenantName(CmdType cmdType, const QString& currentCovenantName, const QString& newCovenantName)
{
    QString tableName = _GetTableName(TableType::Commands, cmdType);

    DB_UPDATE(tableName, QString("Covenant='%1'").arg(newCovenantName), QString("Covenant='%1'").arg(currentCovenantName));
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

    DB_QUERY_PTR query = DB_SELECT(tableName, "*", QString("Answer IN (SELECT Answer FROM %1 "
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
    // Answer should have at least 1 character to be added
    if (!answer.isEmpty())
    {
        QString tableName = _GetTableName(TableType::Answers, cmdType);
        QString guardedAnswer = answer;
        guardedAnswer.replace("'", "''");

        if (DB_INSERT(tableName, QString("NULL, '%1', '%2'").arg(cmdName).arg(guardedAnswer)))
        {
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
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandDBHelper::EditAnswer(CmdType cmdType, int id, const QString &answer)
{
    QString tableName  = _GetTableName(TableType::Answers, cmdType);
    QString cmdName;
    QString guardedAnswer = answer;
    guardedAnswer.replace("'", "''");

    if (DB_UPDATE(tableName, QString("Answer='%1'").arg(guardedAnswer), QString("Id=%1").arg(id)))
    {
        DB_QUERY_PTR query = DB_SELECT(tableName, "Name", QString("Id=%1").arg(id));
        if ((query != nullptr) && (query->first()))
        {
            cmdName = query->value("Name").toString();
            if (!cmdName.isEmpty())
            {
                switch (cmdType)
                {
                case CmdType::StreamerCmd:
                    emit CustomCmdAnswerEdited(cmdName, id);
                    break;
                case CmdType::CovenantCmd:
                    emit CustomCovCmdAnswerEdited(cmdName, id);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CustomCommandDBHelper::DeleteAnswer(CmdType cmdType, int id)
{
    QString tableName  = _GetTableName(TableType::Answers, cmdType);
    QString cmdName;

    DB_QUERY_PTR query = DB_SELECT(tableName, "Name", QString("Id=%1").arg(id));
    if ((query != nullptr) && (query->first()))
    {
        if (DB_DELETE(tableName, QString("Id=%1").arg(id)))
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
        error = QString("Table '%1' was not created.").arg(mainTableName + "s");
    }

    // Additional table
    if (!DB_CREATE_TABLE((mainTableName+"Answers"), "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                    "Name TEXT NOT NULL,"
                                                    "Answer TEXT NOT NULL"))
    {
        error = QString("Table '%1' was not created.").arg(mainTableName + "Answers");
    }

    if (!DB_CREATE_INDEX((mainTableName+"Answers"), (indexPrefix + "Answer_Index"), "Answer"))
    {
        error = QString("Index '%1' was not created for table '%2'.").arg(mainTableName + "Answers").arg(indexPrefix + "Answer_Index");
    }

    if (!DB_CREATE_INDEX((mainTableName+"Answers"), (indexPrefix + "Name_Index"), "Name"))
    {
        error = QString("Index '%1' was not created for table '%2'.").arg(mainTableName + "Answers").arg(indexPrefix + "Name_Index");
    }

    return error;
}

///////////////////////////////////////////////////////////////////////////
