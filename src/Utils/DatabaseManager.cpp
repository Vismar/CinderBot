#include "DatabaseManager.hpp"
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

bool DatabaseManager::CreateTable(const QString& tableName, const QString& columns)
{
    bool result = true;
    QSqlQuery query;
    // Prepare command
    query.prepare(QString("CREATE TABLE IF NOT EXISTS %1 (%2);").arg(tableName).arg(columns));
    if (!query.exec())
    {
        // If command failed, return error
        result = false;
        qDebug() << "Database error: " << query.lastError().text();
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
        qDebug() << "Database error: " << query.lastError().text();
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
    qDebug() << "Database error: " << query->lastError().text();
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
        qDebug() << "Database error: " << query.lastError().text();
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
        qDebug() << "Database error: " << query.lastError().text();
    }
    else
    {
        // Notify observers
        emit OnDeleteEvent(tableName);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
