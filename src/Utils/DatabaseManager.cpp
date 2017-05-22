#include "DatabaseManager.hpp"
#include <QStringList>
#include <QDebug>

///////////////////////////////////////////////////////////////////////////

DatabaseManager::DatabaseManager() : QObject(0)
{
    _database = QSqlDatabase::addDatabase("QSQLITE");
    _database.setDatabaseName("CinderBotDatabase.sqlite");
}

///////////////////////////////////////////////////////////////////////////

DatabaseManager::~DatabaseManager()
{
    _database.close();
}

///////////////////////////////////////////////////////////////////////////

DatabaseManager& DatabaseManager::Instance()
{
    static DatabaseManager instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString DatabaseManager::Initialize()
{
    // Try to open database
    QString result("OK");
    if (!_database.open())
    {
        // If something goes wrong, return error
        result = _database.lastError().text();
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::CreateIndex(const QString& tableName, const QString& indexName, const QString& columns)
{
    bool result = true;

    QString command;
    QSqlQuery query;
    // Prepare command for whole table or specified columns in specified table
    if (columns.isEmpty())
    {
        command = QString("CREATE INDEX IF NOT EXISTS %2 ON %1;").arg(tableName).arg(indexName);
    }
    else
    {
        command = QString("CREATE INDEX IF NOT EXISTS %2 ON %1 (%3);").arg(tableName).arg(indexName).arg(columns);
    }
    // If command failed. return error
    if (!query.exec(command))
    {
        result = false;
        qDebug() << "Database error(CreateIndex): " << query.lastError();
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::CreateTable(const QString& tableName, const QString& columns)
{
    bool result = true;

    std::shared_ptr<QSqlQuery> queryToCheckTable = DB_SELECT("sqlite_master",
                                                             "COUNT(*)",
                                                             QString("type = 'table' AND name = '%1'").arg(tableName));
    if (queryToCheckTable != NULL)
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
                qDebug() << "Database error(CreateTable): " << query.lastError().text();
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
                bool columnsExists[columnList.size()];
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
                            qDebug() << "Database error(CreateTable): " << alterTableQuery.lastError().text();
                        }
                    }
                }
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Insert(const QString& tableName, const QString& recordValues, bool ignore)
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
        qDebug() << "Database error(Insert): " << query.lastError().text();
    }
    else
    {
        // Notify observers
        emit OnInsertEvent(tableName);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

std::shared_ptr<QSqlQuery> DatabaseManager::Select(const QString& tableName, const QString& columnNames, const QString& conditions)
{
    std::shared_ptr<QSqlQuery> query = std::make_shared<QSqlQuery>();
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
    query->prepare(command);
    if (query->exec())
    {
        return query;
    }
    // If command failed, return error
    qDebug() << "Database error(Select): " << query->lastError().text();
    return NULL;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Update(const QString& tableName, const QString& columnValues, const QString& conditions)
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
        qDebug() << "Database error(Update): " << query.lastError().text();
    }
    else
    {
        // Notify observers
        emit OnUpdateEvent(tableName);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Delete(const QString& tableName, const QString& conditions)
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
        qDebug() << "Database error(Delete): " << query.lastError().text();
    }
    else
    {
        // Notify observers
        emit OnDeleteEvent(tableName);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
