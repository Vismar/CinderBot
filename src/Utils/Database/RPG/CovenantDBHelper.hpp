/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Utils/Database/DatabaseManager.hpp"

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
 * \brief Simple enumeration of covenant parameters.
 */
enum class CovenantParameter
{
    Name,
    Leader,
    Description,
    CmdSlots,
    Level,
    Exp,
    MaxMembers
};

/*!
 * \brief Container that store all covenant parameters.
 * 
 * Containers by default stores basic values for each parameter, 
 * but can be filled with actual info by query from database or manually.
 */
struct CovParams
{
    QString ToString() const;
    void FillDataFromQuery(DB_QUERY_PTR query);

    QString Name = "";
    QString Leader = "";
    QString Description = "";
    int CmdSlots = 1;
    int Level = 1;
    int Exp = 0;
    int MaxMembers = 10;
};

/*!
 * \brief Helper class designed to facilitate the work with the database for covenant data.
 * 
 * Handles creating database tables, provides simple interface to create/disband covenants, do other stuff.
 * Updating covenant parameters, checking leadership and etc.
 */
class CovenantDBHelper : public QObject
{
    Q_OBJECT
public:
    static CovenantDBHelper &Instance();

    /*!
     * \brief Initializes all database tables.
     * \return Error message if something goes wrong, otherwise - 'OK'.
     */
    static QString InitializeTables();

    /*!
     * \brief Creates covenant with specified name and leader.
     * \param covenantName - name of covenant that should be created.
     * \param leaderName - name of a creater.
     * \return True if covenant was created. Otherwise - false.
     */
    static bool CreateCovenant(const QString &covenantName, const QString &leaderName);
    /*!
     * \brief Disbands covenant with specified name.
     * \param covenantName - name of covenant that should be disbanded.
     * \return True if covenant was disbanded. Otherwise - false.
     */
    static bool DisbandCovenant(const QString &covenantName);
    /*!
     * \brief Renames specified covenant.
     * \param covenantName - name of covenant that should be renamed.
     * \param newCovenantName - new covenant name.
     * \return True if covenant was renamed. Otherwise - false.
     */
    static bool RenameCovenant(const QString &covenantName, const QString &newCovenantName);

    /*!
     * \brief Grabs required parameter from specified covenant.
     * \param param - parameter name.
     * \param covenantName - name of covenant.
     * \return Parameter value. 
     */
    static QVariant GetParameter(CovenantParameter param, const QString &covenantName);
    /*!
     * \brief Sets covenant paramter to the specified value.
     * \param param - parameter name.
     * \param value - new value for specified parameter.
     * \param covenantName - name of covenant.
     * \return True if paramter was setted. Otherwise - false.
     */
    static bool SetParameter(CovenantParameter param, const QString &value, const QString &covenantName);
    /*!
     * \brief Grabs all parameters from specified covenant.
     * \param covenantName - name of covenant.
     * \return Container that stores all covenant paramters.
     */
    static CovParams GetParams(const QString &covenantName);
    /*!
     * \brief Sets all parameters to specified covenant.
     * \param params - covenant paramters.
     * \return True if parameters was setted. Otherwise - false.
     */
    static bool SetParams(CovParams params);

    /*!
     * \brief Grabs all covenant names.
     * \return List of covenant names.
     */
    static QStringList GetCovenants();
    /*!
     * \brief Checks if specified user is leader of its covenant.
     * \param userID - id of user.
     * \return True if specified user is leader of its covenant.
     */
    static bool CheckLeadership(int userID);
    /*!
     * \brief Checks if specified user is leader of its covenant
     * \param userName - name of user.
     * \return True if specified user is leader of its covenant.
     */
    static bool CheckLeadership(const QString &userName);
    /*!
     * \brief Checks if specified covenant even exist.
     * \param covenantName - name of covenant.
     * \return True if covenant exist, Otherwise - false.
     */
    static bool IsCovenantExist(const QString &covenantName);
    /*!
     * \brief Tries to add specified amount of experience to certain covenant.
     * \param expValue - amount of experience that should be added to covenant.
     * \param covenantName - name of covenant.
     */
    static void GainExpToCovenant(int expValue, const QString &covenantName);
};

}
}