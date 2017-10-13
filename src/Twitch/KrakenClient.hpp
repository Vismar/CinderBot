/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include <QNetworkReply>
#include <QTimer>
#include <QQueue>

/*!
 * \brief Contains all thing related to Twitch stuff.
 */
namespace Twitch
{

/*!
 * \brief Simple enumeration of kraken parameters that can be stored or obtained.
 */
enum KrakenParameter
{
    ClientID,
    BotUserID,
    KnownBotStatus,
    ChannelUserID,
    ChannelTitle,
    ChannelGame,
    ChannelPartnerStatus,
    ChannelViews,
    ChannelFollowers,
    ChannelSubscribers,
    ChannelBroadcasterType,
    StreamOn,
    StreamType,
    StreamViewers,
    StreamResolution,
    StreamFPS
};

// Forward declaration
class KrakenResponse;

/*!
 * \brief Client that thrying to obtain all kind of information from Kraken API that are required for bot.
 */
class KrakenClient : public QObject
{
    Q_OBJECT
public:
    explicit KrakenClient(QObject *parent = nullptr);

    /*!
     * \brief Create an instance of KrakenClient.
     * \return Will create and then return static variable.
     */
    static KrakenClient &Instance();
    /*!
     * \brief Initialize default values and some other things.
     * \return "OK" if everythings goes right. Otherwise will return error message.
     */
    QString Initialize();

    /*!
     * \brief Grabs parameter value and returns it as QVariant.
     * \param param - parameter that should be returned.
     * \return Parameter value as QVariant. If such parameter do not exist, will be returned "empty" QVariant.
     */
    QVariant GetParameter(KrakenParameter param) const;

signals:
    /*!
     * \brief Signal only produced when 
     * \param param - parameter that was changed.
     * \param value - new value of specified parameter.
     */
    void ParameterChanged(KrakenParameter param, const QVariant &value);

private slots:
    /*!
     * \brief Sends oldest request in queue due to timer.
     */
    void _SendRequestDueToTimer();
    /*!
     * \brief Reads new response from Kraken API.
     * \param reply - pointer to reply.
     */
    void _ReadResponse(QNetworkReply *reply);

    /*!
     * \brief Initializes all possible parameters.
     */
    void _InitializeParameters();

private:
    /*!
    * \brief Sets specified value to certain parameter.
    * \param param - parameter that should be updated.
    * \param value - new value for specified parameter.
    * \return True if parameter was changed, otherwise, if new value for parameter was the same, will return false.
    */
    bool _SetParameter(KrakenParameter param, const QVariant &value);

    /*!
    * \brief Adds request to queue.
    * \param request - request that will be added to queue of requests.
    */
    void _AddRequestToQueue(const QString &request);
    /*!
    * \brief Sends request to Kraken API.
    * \param request - request that will be sent.
    */
    void _SendRequest(const QString &request) const;

    /*** KrakenAPI requests ***/
    /*!
     * \brief Tries to get bot user id.
     * 
     * If bot user id is not gained, tries to get it.
     */
    void _InitializeBotUserID();
    /*!
     * \brief Tries to get known bot status.
     * 
     * To get known bot status bot need to know bot user id.
     */
    void _InitializeKnownBotStatus();
    /*!
     * \brief Tries to get channel user id.
     * 
     * If channel user id is not gained, tries to get it.
     */
    void _InitializeChannelUserID();
    /*!
     * \brief Tries to get channel info.
     *
     * If channel info not gained, tries to get it.
     */
    void _UpdateChannelInfo();
    /*!
     * \brief Tries to get stream info.
     *
     * Tries to get stream info to know when stream is up and how many viewers currently watching the stream.
     */
    void _UpdateStreamInfo();
    /*!
     * \brief Tries to get subscriber counter.
     *
     * Tries to get subscribers counter to know how many broadcaster have.
     */
    void _UpdateSubscribers();
    
    /*** Handlers for responses ***/
    /*!
     * \brief Handles error message from Kraken API.
     * \param response - json object of response.
     */
    void _HandleErrorResponse(const KrakenResponse &response) const;
    /*!
     * \brief Handles user info message from Kraken API.
     * \param response - json object of response.
     */
    void _HandleUserInforesponse(const KrakenResponse &response);
    /*!
     * \brief Handles known bot status message from Kraken API.
     * \param response - json object of response.
     */
    void _HandleKnownBotStatus(const KrakenResponse &response);
    /*!
     * \brief Handles channel info message from Kraken API.
     * \param response - json object of response.
     */
    void _HandleChannelInfo(const KrakenResponse &response);
    /*!
     * \brief Handles stream info message from Kraken API.
     * \param response - json object of response.
     */
    void _HandleStreamInfo(const KrakenResponse &response);
    /*!
    * \brief Handles followers message from Kraken API.
    * \param response - json object of response.
    */
    void _HandleFollowers(const KrakenResponse &response) const;
    /*!
     * \brief Handles subscribers message from KrakenAPI.
     * \param response - json object of response.
     */
    void _HandleSubscribers(const KrakenResponse &response);

    /*** Handlers for changing specific parameters ***/
    /*!
     * \brief Handles change of kraken parameter ChannelFollowers.
     * \param newTotalFollowers - new value of parameter.
     * 
     * Defines the difference beetween old and new values, after that creates requests to grab needed users from KrakenAPI.
     */
    void _HandleParameterChangeChannelFollower(int newTotalFollowers);

    /*! Network manager which sends and receives messages to/from Kraken API. */
    QNetworkAccessManager *_networkManager;
    /*! Timer that displays how often bot will send request to Kraken API. */
    QTimer *_limitTimer;
    /*! Timer that displays how often bot will try to send new requests to Kraken API. */
    QTimer *_requestTimer;
    /*! Queue of requests. */
    QQueue<QString> _requests;
    /*! Hash table of parameters obtained from Kraken API. */
    QHash<KrakenParameter, QVariant> _krakenParameters;
};

}