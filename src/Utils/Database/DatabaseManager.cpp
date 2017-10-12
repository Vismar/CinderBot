/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "DatabaseManager.hpp"
#include <QStringList>
#include <QDir>
#include "Utils/Logger.hpp"

using namespace Utils;
using namespace Database;

#define DB_FOLDER    "data"
#define DB_FILE_NAME "CinderBotDatabase.sqlite"

///////////////////////////////////////////////////////////////////////////

DatabaseManager::DatabaseManager() : QObject(nullptr), _transactionOn(false)
{
    _database = QSqlDatabase::addDatabase("QSQLITE");
    QString dbName = QString("./%1/%2").arg(DB_FOLDER).arg(DB_FILE_NAME);
    _database.setDatabaseName(dbName);
}

///////////////////////////////////////////////////////////////////////////

DatabaseManager::~DatabaseManager()
{
    _database.close();
}

///////////////////////////////////////////////////////////////////////////

DatabaseManager &DatabaseManager::Instance()
{
    static DatabaseManager instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString DatabaseManager::Initialize()
{
    // Try to open database
    QString result("OK");
    // Check directory
    QDir dir(DB_FOLDER);
    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            LOG(LogError, this->metaObject()->className(), __func__, QString("Folder %1 cannot be crated.").arg(DB_FOLDER));
            result = "Cannot create a data folder!";
        }
    }
    if ((result == "OK") && (!_database.open()))
    {
        // If something goes wrong, return error
        result = _database.lastError().text();
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::StartTransaction()
{
    bool transactionStarted(false);

    if (!_transactionOn)
    {
        transactionStarted = _database.transaction();
        _transactionOn = transactionStarted;
    }
    
    // Log error
    if (!transactionStarted)
    {
        LOG(LogError, this->metaObject()->className(), __func__, QString("Error while trying to start transaction.\nError message: %1").arg(_database.lastError().text()));
    }

    return transactionStarted;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::EndTransaction()
{
    bool commited(false);

    if (_transactionOn)
    {
        commited = _database.commit();
        _transactionOn = !commited; // If transaction succeeded we should turn off transaction flag, otherwise - turn on
    }
    
    // Log error
    if (!commited)
    {
        LOG(LogError, this->metaObject()->className(), __func__, QString("Error while trying to end transaction.\nError message: %1").arg(_database.lastError().text()));
    }

    return commited;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::CreateIndex(const QString &tableName, const QString &indexName, const QString &columns, bool unique) const
{
    bool result = true;

    QString command;
    QSqlQuery query;

    // Prepare command for whole table or specified columns in specified table
    if (columns.isEmpty())
    {
        command = QString("CREATE UNIQUE INDEX IF NOT EXISTS %2 ON %1;").arg(tableName).arg(indexName);
    }
    else
    {
        command = QString("CREATE UNIQUE INDEX IF NOT EXISTS %2 ON %1 (%3);").arg(tableName).arg(indexName).arg(columns);
    }

    if (!unique)
    {
        command.replace("UNIQUE", "");
    }

    // If command failed. return error
    if (!query.exec(command))
    {
        result = false;
        LOG(LogError, "", Q_FUNC_INFO,
            QString("Values: tableName='%1'; indexName='%2'; columns=' %3 ';\n"
                    "Error message: %4").arg(tableName).arg(indexName).arg(columns).arg(query.lastError().text()));
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::CreateTable(const QString &tableName, const QString &columns) const
{
    bool result = true;

    std::shared_ptr<QSqlQuery> queryToCheckTable = DB_SELECT("sqlite_master",
                                                             "COUNT(*)",
                                                             QString("type = 'table' AND name = '%1'").arg(tableName));
    if (queryToCheckTable != nullptr)
    {
        queryToCheckTable->first();
        // If table do not exist, create it
        if (queryToCheckTable->value(0).toInt() == 0)
        {
            QSqlQuery query;
            // Prepare command
            query.prepare(QString("CREATE TABLE %1 (%2);").arg(tableName).arg(columns));
            if (!query.exec())
            {
                // If command failed, return error
                result = false;
                LOG(LogError, "", Q_FUNC_INFO,
                    QString("Values: tableName='%1'; columns=' %2 ';\n"
                            "Error message: %3").arg(tableName).arg(columns).arg(query.lastError().text()));
            }
        }
        // If table exist, check if all columns are added
        else
        {
            QSqlQuery pragmaQuery;
            pragmaQuery.prepare(QString("PRAGMA table_info(%1)").arg(tableName));
            if (pragmaQuery.exec())
            {
                QStringList columnList = columns.split(",");
                bool *columnsExists = new bool[columnList.size()];
                // We have to initialize this array in that way
                // because gcc do not allow to inititalize array with value which do not known in compile time
                for (int i = 0; i < columnList.size(); ++i)
                {
                    columnsExists[i] = false;
                }
                // Check if columns exist in current table
                while (pragmaQuery.next())
                {
                    for (int i = 0; i < columnList.size(); ++i)
                    {
                        if (columnList.at(i).startsWith(pragmaQuery.value("name").toString() + " "))
                        {
                            columnsExists[i] = true;
                        }
                    }
                }
                // Parse list of columns that should exist and create if columns do not exist
                for (int j = 0; j < columnList.size(); ++j)
                {
                    if (!columnsExists[j])
                    {
                        QSqlQuery alterTableQuery;
                        //qDebug() << alterTableQuery.prepare(command);
                        if (!alterTableQuery.exec(QString("ALTER TABLE %1 ADD COLUMN %2;").arg(tableName).arg(columnList.at(j))))
                        {
                            // If command failed, return error
                            result = false;
                            LOG(LogError, "", Q_FUNC_INFO,
                                QString("Values: tableName='%1'; columns=' %2 ';\n"
                                        "Error message: %3").arg(tableName).arg(columns).arg(alterTableQuery.lastError().text()));
                        }
                    }
                }
                delete[] columnsExists;
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Insert(const QString &tableName, const QString &recordValues, bool ignore)
{
    bool result = true;
    QSqlQuery query;
    // Insert IGNORE if it was specififed
    QString orIgnore = ignore ? "OR IGNORE" : "";
    // Prepare command
    query.prepare(QString("INSERT %1 INTO %2 VALUES (%3);").arg(orIgnore).arg(tableName).arg(recordValues));
    if (!query.exec())
    {
        // If command failed, return error
        result = false;
        LOG(LogError, "", Q_FUNC_INFO, QString("Values: tableName='%1'; recordValues=' %2 '; ignore='%3';\n"
                                               "Error message: %4").arg(tableName).arg(recordValues).arg(ignore).arg(query.lastError().text()));
    }
    else
    {
        // Notify observers
        emit OnInsertEvent(tableName);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

DB_QUERY_PTR DatabaseManager::Select(const QString &tableName, const QString &columnNames, const QString &conditions) const
{
    DB_QUERY_PTR query = std::make_shared<QSqlQuery>();
    QString command;
    // Prepare command
    if (conditions.isEmpty())
    {
        command = QString("SELECT %1 FROM %2;").arg(columnNames).arg(tableName);
    }
    else
    {
        command = QString("SELECT %1 FROM %2 WHERE %3;").arg(columnNames).arg(tableName).arg(conditions);
    }
    if (query->exec(command))
    {
        return query;
    }
    // If command failed, return error
    LOG(LogError, "", Q_FUNC_INFO,
        QString("Values: tableName='%1'; columnNames=' % 2'; conditions=' %3 ';\n"
                "Error message: %4").arg(tableName).arg(columnNames).arg(conditions).arg(query->lastError().text()));

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Update(const QString &tableName, const QString &columnValues, const QString &conditions)
{
    bool result = true;
    QSqlQuery query;
    QString command;
    // Prepare command
    if (conditions.isEmpty())
    {
        command = QString("UPDATE %1 SET %2;").arg(tableName).arg(columnValues);
    }
    else
    {
        command = QString("UPDATE %1 SET %2 WHERE %3;").arg(tableName).arg(columnValues).arg(conditions);
    }
    query.prepare(command);
    if (!query.exec())
    {
        // If command failed, return error
        result = false;
        LOG(LogError, "", Q_FUNC_INFO,
            QString("Values: tableName='%1'; columnValues=' %2 '; conditions=' %3 ';\n"
                    "Error message: %4").arg(tableName).arg(columnValues).arg(conditions).arg(query.lastError().text()));
    }
    else
    {
        // Notify observers
        emit OnUpdateEvent(tableName);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Delete(const QString &tableName, const QString &conditions)
{
    bool result = true;
    QSqlQuery query;
    QString command;
    // Prepare command
    if (conditions.isEmpty())
    {
        command = QString("DELETE FROM %1;").arg(tableName);
    }
    else
    {
        command = QString("DELETE FROM %1 WHERE %2;").arg(tableName).arg(conditions);
    }
    query.prepare(command);
    if (!query.exec())
    {
        // If command failed, return error
        result = false;
        LOG(LogError, "", Q_FUNC_INFO,
            QString("Values: tableName='%1'; conditions=' %2 ';\n"
                    "Error message: %3").arg(tableName).arg(conditions).arg(query.lastError().text()));
    }
    else
    {
        // Notify observers
        emit OnDeleteEvent(tableName);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
