#pragma once
#include <QtSql>
#include <memory>

class DatabaseManager
{
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
     * Insert new record to the database
     * \param(IN) tableName - name of a table in which new record should be added
     * \param(IN) recordValues - values for a new record
     * \return true if command was successful
     */
    bool Insert(const QString& tableName, const QString& recordValues);
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
     * \param(IN) tableName - name of a table in which new record should be added
     * \param(IN) conditions - conditions for selection from the table
     * \return true if command was successful
     */
    bool Delete(const QString& tableName, const QString& conditions = "");

private:
    /*! Database which is used to store all data */
    QSqlDatabase _database;
};
