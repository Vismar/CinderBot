#include "DatabaseManager.hpp"
#include <QDebug>

///////////////////////////////////////////////////////////////////////////

DatabaseManager::DatabaseManager()
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
    QString result("OK");
    if (!_database.open())
    {
        result = _database.lastError().text();
    }
    else
    {
        // Create tables here
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Insert(const QString& tableName, const QString& recordValues)
{
    bool result = true;
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 VALUES (%2);").arg(tableName).arg(recordValues));
    qDebug() << QString("INSERT INTO %1 VALUES (%2);").arg(tableName).arg(recordValues);
    if (!query.exec())
    {
        result = false;
        qDebug() << "Database error: " << query.lastError().text();
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

std::shared_ptr<QSqlQuery> DatabaseManager::Select(const QString& tableName, const QString& columnNames, const QString& conditions)
{
    std::shared_ptr<QSqlQuery> query = std::make_shared<QSqlQuery>();
    QString command;
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

    qDebug() << "Database error: " << query->lastError().text();
    return NULL;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Update(const QString& tableName, const QString& columnValues, const QString& conditions)
{
    bool result = true;
    QSqlQuery query;
    QString command;
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
        result = false;
        qDebug() << "Database error: " << query.lastError().text();
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

bool DatabaseManager::Delete(const QString& tableName, const QString& conditions)
{
    bool result = true;
    QSqlQuery query;
    QString command;
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
        result = false;
        qDebug() << "Database error: " << query.lastError().text();
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
