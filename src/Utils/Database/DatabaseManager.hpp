/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QtSql>
#include <memory>

#define DB_MANAGER      DatabaseManager::Instance()
#define DB_CREATE_INDEX DatabaseManager::Instance().CreateIndex
#define DB_CREATE_TABLE DatabaseManager::Instance().CreateTable
#define DB_INSERT       DatabaseManager::Instance().Insert
#define DB_SELECT       DatabaseManager::Instance().Select
#define DB_UPDATE       DatabaseManager::Instance().Update
#define DB_DELETE       DatabaseManager::Instance().Delete

#define DB_QUERY_PTR    std::shared_ptr<QSqlQuery>

/*!
 * \brief Contains all utility classes.
 */
namespace Utils
{
/*!
 * \brief Contains all database related classes.
 */
namespace Database
{

/*!
 * \brief Manager that holds all work with database and provide functionality to work directly with it.
 * 
 * Manager inititalizes database, creates everything that are needed for correct work. 
 * Provide functionality to create tables and indexes; insert, update, delete and select to/from tables.
 */
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    DatabaseManager();
    ~DatabaseManager();

    /*!
     * \brief Initializes and creates instance of database manaager
     * \return reference to static database manager
     */
    static DatabaseManager &Instance();

    /*!
     * \brief Tries to open and initialize database.
     * \return Error code if something goes wrong, or OK, if everything is fine.
     * 
     * Checks directory and tries to create. After that tries to open/create database. 
     * If any error has appeared at any step, will return error. Otherwise - "OK"
     */
    QString Initialize();

    /*!
     * \brief Tries to start transaction for multiple queries.
     * \return True if database has this feature and transaction was started. Otherwise - false.
     */
    bool StartTransaction();
    /*!
     * \brief Tries to end transaction and commits all queries to database.
     * \return True if commit was successfull and previously transaction was started. Otherwise - false.
     */
    bool EndTransaction();

    /*!
     * \brieef Creates index on specified table (and columns if specified).
     * \param tableName - name of table.
     * \param indexName - name of index.
     * \param columns - columns which should be indexed.
     * \return True if index successfully created.
     */
    bool CreateIndex(const QString &tableName, const QString &indexName, const QString &columns = "") const;
    /*!
     * \brief Tries to create table if it not exist.
     * \param tableName - name of a table which should be created.
     * \param columns - columns with all parameters that should be created for specified table.
     * \return True if command was successful.
     */
    bool CreateTable(const QString &tableName, const QString &columns) const;
    /*!
     * \brief Inserts new record to the database.
     * \param tableName - name of a table in which new record should be added.
     * \param recordValues - values for a new record.
     * \return True if command was successful.
     */
    bool Insert(const QString &tableName, const QString &recordValues, bool ignore = false);
    /*!
     * \brief Selects specified records from database.
     * \param tableName - name of a table in which new record should be added.
     * \param columnNames - name of columns which you want to be returned.
     * \param conditions - conditions for selection from the table.
     * \return Shared pointer to a query if command was successful, will return NULL of command failed.
     */
    std::shared_ptr<QSqlQuery> Select(const QString &tableName, const QString &columnNames, const QString &conditions = "") const;
    /*!
     * \brief Updates existing record.
     * \param tableName - name of a table in which new record should be added.
     * \param columnValues - values which will be assigned to specififed record.
     * \param conditions - conditions for selection from the table.
     * \return True if command was successful.
     */
    bool Update(const QString &tableName, const QString &columnValues, const QString &conditions = "");
    /*!
     * \brief Deletes existing record from database.
     * \param tableName - name of a table in which record should be deelted.
     * \param conditions - conditions for selection from the table.
     * \return True if command was successful.
     */
    bool Delete(const QString &tableName, const QString &conditions = "");

signals:
    /*!
     * \brief Event for any succesfull insert command.
     * \param tableName - name of table for which operation was executed.
     */
    void OnInsertEvent(const QString &tableName);
    /*!
     * \brief Event for any succesfull update command.
     * \param tableName - name of table for which operation was executed.
     */
    void OnUpdateEvent(const QString &tableName);
    /*!
     * \brief Event for any succesfull delete command.
     * \param tableName - name of table for which operation was executed.
     */
    void OnDeleteEvent(const QString &tableName);

private:
    /*! Flag to define when transaction is on. */
    bool _transactionOn;
    /*! Database which is used to store all data. */
    QSqlDatabase _database;
};

}
}
