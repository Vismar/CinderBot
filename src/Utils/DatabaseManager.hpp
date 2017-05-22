#pragma once
#include <QObject>
#include <QtSql>
#include <memory>

#define DB_CREATE_INDEX DatabaseManager::Instance().CreateIndex
#define DB_CREATE_TABLE DatabaseManager::Instance().CreateTable
#define DB_INSERT       DatabaseManager::Instance().Insert
#define DB_SELECT       DatabaseManager::Instance().Select
#define DB_UPDATE       DatabaseManager::Instance().Update
#define DB_DELETE       DatabaseManager::Instance().Delete

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * Default constructor
     */
    DatabaseManager();
    /*!
     * Desctructor
     */
    ~DatabaseManager();
    /*!
     * Initialize and create instance of database manaager
     * \return reference to static database manager
     */
    static DatabaseManager& Instance();

    /*!
     * Try to open and initialize database
     * \return error code if something goes wrong, or OK, if everything is fine
     */
    QString Initialize();

    /*!
     * Create index on specified table (and columns if specified)
     * \param tableName - name of table
     * \param indexName - name of index
     * \param columns - columns which should be indexed
     * \return true if index successfully created
     */
    bool CreateIndex(const QString& tableName, const QString& indexName, const QString& columns = "");
    /*!
     * Try to create table if it not exist
     * \param(IN) tableName - name of a table which should be created
     * \param(IN) columns - columns with all parameters that should be created for specified table
     * \return true if command was successful
     */
    bool CreateTable(const QString& tableName, const QString& columns);
    /*!
     * Insert new record to the database
     * \param(IN) tableName - name of a table in which new record should be added
     * \param(IN) recordValues - values for a new record
     * \return true if command was successful
     */
    bool Insert(const QString& tableName, const QString& recordValues, bool ignore = false);
    /*!
     * \brief Select
     * \param(IN) tableName - name of a table in which new record should be added
     * \param(IN) columnNames - name of columns which you want to be returned
     * \param(IN) conditions - conditions for selection from the table
     * \return shared pointer to a query if command was successful, will return NULL of command failed
     */
    std::shared_ptr<QSqlQuery> Select(const QString& tableName, const QString& columnNames, const QString& conditions = "");
    /*!
     * \brief Update
    * \param(IN) tableName - name of a table in which new record should be added
     * \param(IN) columnValues - values which will be assigned to specififed record
     * \param(IN) conditions - conditions for selection from the table
     * \return true if command was successful
     */
    bool Update(const QString& tableName, const QString& columnValues, const QString& conditions = "");
    /*!
     * \brief Delete
     * \param(IN) tableName - name of a table in which record should be deelted
     * \param(IN) conditions - conditions for selection from the table
     * \return true if command was successful
     */
    bool Delete(const QString& tableName, const QString& conditions = "");

signals:
    /*!
     * Event for any succesfull insert command
     * \param tableName - name of table for which operation was executed
     */
    void OnInsertEvent(const QString& tableName);
    /*!
     * Event for any succesfull update command
     * \param tableName - name of table for which operation was executed
     */
    void OnUpdateEvent(const QString& tableName);
    /*!
     * Event for any succesfull delete command
     * \param tableName - name of table for which operation was executed
     */
    void OnDeleteEvent(const QString& tableName);

private:
    /*! Database which is used to store all data */
    QSqlDatabase _database;
};
