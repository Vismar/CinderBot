/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>

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
 * \brief Simple container to store quote data.
 */
struct QuoteData
{
    /*! Quote number */
    int QuoteNumber = -1;
    /*! Quote text data */
    QString Quote;
};

/*!
 * \brief Helper class designed to facilitate the work with the database for quotes.
 * 
 * Handles the creating database tables, provides simple interface to create/update/edit quotes.
 * In addition, emits events about any changes in quote database table.
 */
class QuoteDBHelper : public QObject
{
    Q_OBJECT
public:
    static QuoteDBHelper &Instance();

    /*!
     * \brief Intitializes database table.
     * \return "OK" if everything went good, otherwise error message.
     * 
     * Initializes database table. Should be executed before running application.
     */
    static QString InitializeTables();

    /*!
     * \brief Returns max quote number.
     * \return Selects last quote and returns its number.
     */
    static int GetMaxQuoteNumber();
    /*!
     * \brief Adds new quote to database.
     * \param quote - Text of quote that should be added.
     * \return Number of added quote.
     */
    int AddQuote(const QString &quote);
    /*!
     * \brief Deletes quote.
     * \param quoteNumber - Number of quote that will be deleted.
     * \return True, if quote was deleted.
     */
    bool DeleteQuote(int quoteNumber);
    /*!
     * \brief Replaces qutoe text by new data.
     * \param quoteNumber - Number of quote that should be updated.
     * \param newQuote - New quote text data.
     * \return True, if quote edited.
     */
    bool EditQuote(int quoteNumber, const QString &newQuote);
    /*!
     * \brief Grabs all ids of all quotes.
     * \return Array of quote ids.
     */
    static QVector<int> GetQuoteIds();
    /*!
     * \brief Grabs specified quote.
     * \param quoteNumber - Number of qutoe that should be grabed.
     * \return Quote text data if such qutoe exists. Otherwise empty string will be returned.
     */
    QString GetQuote(int quoteNumber) const;
    /*!
     * \brief Grabs random quote.
     * \return Randomly selected quote. 
     */
    static QString GetRandomQuote();
    /*!
     * \brief Grabs qutoe data by specified id.
     * \param id - Id of quote.
     * \return Quote data that contains quote number and quot itself.
     */
    static QuoteData GetQuoteData(int id);

signals:
    /*!
     * \brief Event produced when a quote was added.
     * \param quoteId - Id of added quote.
     */
    void QuoteAdded(int quoteId);
    /*!
     * \brief Event produced when a quote was deleted.
     * \param quoteId - Id of deleted quote.
     */
    void QuoteDeleted(int quoteId);
    /*!
     * \brief Event produced when a quote was updated.
     * \param quoteNumber - Id of updated quote.
     */
    void QuoteUpdated(int quoteNumber);

private:
    /*!
     * \brief Refreshes all quote numbers that comes after the specified one.
     * \param quoteNumber - Number of quote from which all quote numbers should be refreshed.
     * 
     * This function refreshes all quote numbers, but its executed only when a quote was deleted.
     */
    void _RefreshQuoteNumbersFrom(int quoteNumber) const;
};

}
}