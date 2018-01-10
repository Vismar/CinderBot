/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Twitch/KrakenClient.hpp"

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
 * \brief Simple enumeration to define type of records in analytics table
 */
enum class AnalyticsRecordType
{
    Undefined = -1,
    Day = 0,
    Live,
    VOD
};

/*!
 * \brief Container that holds all info about certain record
 */
struct AnalyticsParameters
{
    AnalyticsRecordType RecorType = AnalyticsRecordType::Undefined;
    QString StartOfTheStream = "";
    QString EndOfTheStream = "";
    int TotalFollowers = 0;
};

/*!
 * \brief helper class designed to facilitate the work with the database for Analytics.
 * 
 *  Handles updating and creating analytics records, changes of total followers and etc.
 */
class AnalyticsDBHelper : public QObject
{
    Q_OBJECT
public:
    explicit AnalyticsDBHelper(QObject *parent = nullptr);
    static AnalyticsDBHelper &Instance();

    /*!
     * \brief Initializes database tables.
     * \return String that contains error message if something goes wrong. Otherwise - "OK".
     */
    static QString InitializeTables();
    
    /*!
     * \brief Grabs last total follower number.
     * \return Number of followers from database or 0, if no records were found.
     * 
     * If no records in database about total followers, then will be returned 0. 
     * Otherwise - will be selected from last record if stream is on, or from record for last day.
     */
    static int GetLastNumberOfFollowers();
    /*!
     * \brief Grabs last total subscriber number.
     * \return Number of subscribers from database or 0, if no records were found.
     *
     * If no records in database about total followers, then will be returned 0. 
     * Otherwise - will be selected from last record if stream is on, or from record for last day.
     */
    static int GetLastNumberOfSubscribers();

private slots:
    /*!
     * \brief Simple handles change of certain kraken parameters.
     * \param param - parameter type.
     * value - value of parameter.
     */
    void _HandleKrakenParameterChange(Twitch::KrakenParameter param, const QVariant &value) const;

private:
    /*!
     * \brief Handles change of parameter named 'StreamOn'.
     * \param streamOn - flag that means if stream is live or not.
     */
    void _HandleChangeOfParameterStreamOn(bool streamOn) const;
    /*!
     * \brief Handles change of parameter names 'ChannelFollowers'.
     * \param totalFollowers - number of followers.
     */
    void _HandleChangeOfParameterChannelFollowers(int totalFollowers) const;
    /*!
     * \brief Handles change of parameter named 'ChannelSubscribers'.
     * \param totalSubscribers - number of subscribers.
     */
    void _HandleChangeOfParameterChannelSubscribers(int totalSubscribers) const;

    /*!
     * \brief Checks if record for current day exists in database.
     * \return True, if such record exist, otherwise - false.
     */
    static bool _IsRecordExistForCurrentDay();
    /*!
     * \brief Creates specified type of record.
     * \param recordType - type of record that should be created in database.
     */
    void _CreateRecord(AnalyticsRecordType recordType) const;
};

}
}