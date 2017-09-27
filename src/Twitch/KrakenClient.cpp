/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "KrakenClient.hpp"
#include "Twitch/KrakenResponse.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Logger.hpp"

using namespace Twitch;
using namespace Utils::Configuration;
using namespace Utils;

///////////////////////////////////////////////////////////////////////////

KrakenClient::KrakenClient(QObject* parent) : QObject(parent)
{
    // Initialize network manager
    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, &QNetworkAccessManager::finished,
            this, &KrakenClient::_ReadResponse);

    // Initialize limit timer
    _limitTimer = new QTimer(this);
    _limitTimer->start(500); // 0.5 sec
    connect(_limitTimer, &QTimer::timeout,
            this, &KrakenClient::_SendRequestDueToTimer);

    // Initialize request timer
    _requestTimer = new QTimer(this);
    connect(_requestTimer, &QTimer::timeout,
            this, &KrakenClient::_InitializeParameters);
}

///////////////////////////////////////////////////////////////////////////

KrakenClient& KrakenClient::Instance()
{
    static KrakenClient instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString KrakenClient::Initialize()
{
    QString result("OK");
    // Add ClientID default value
    _SetParameter(KrakenParameter::ClientID, "we0qz5ot41crhkeo1w6mv9t769x1q5");
    
    //_InitializeParameters();
    _requestTimer->start(1000); // 1 sec

    // If network in unaccessible, return error
    if (_networkManager->networkAccessible() != QNetworkAccessManager::Accessible)
    {
        result = "Kraken API could not be retrieved via internet connection.";
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

QVariant KrakenClient::GetParameter(KrakenParameter param) const
{
    return _krakenParameters[param];
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_SendRequestDueToTimer()
{
    // Check if any request in queue
    if (!_requests.isEmpty())
    {
        _SendRequest(_requests.dequeue());
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_ReadResponse(QNetworkReply *reply)
{
    // Read all data from response
    QString response = reply->readAll();

    // Get kraken response type
    KrakenResponse krakenResponse;
    KrakenResponseType responseType = krakenResponse.ParseResponse(response);
    
    // Handle responses
    switch (responseType)
    {
    case KrakenResponseType::Error:
        _HandleErrorResponse(krakenResponse);
        break;
    case KrakenResponseType::UserInfo:
        _HandleUserInforesponse(krakenResponse);
        break;
    case KrakenResponseType::BotStatus:
        _HandleKnownBotStatus(krakenResponse);
        break;
    case KrakenResponseType::ChannelInfo:
        _HandleChannelInfo(krakenResponse);
        break;
    default:
        // Log error
        LOG(LogWarning, QString("Indefined response from Kraken API.\nMessage: %1").arg(response));
        break;
    }

    // Log info
    LOG(LogInfo, QString("Response from Kraken API: %1").arg(response));

    reply->deleteLater();
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_InitializeParameters()
{
    // If network is accessible, try to initialize params
    if (_networkManager->networkAccessible() == QNetworkAccessManager::Accessible)
    {
        _InitializeBotUserID();
        _InitializeKnownBotStatus();
        _InitializeChannelUserID();
        _UpdateChannelInfo();
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_InitializeBotUserID()
{
    // BotUserID param
    if (!_krakenParameters.contains(BotUserID))
    {
        // Only if we have bot user name, try to get bot user id
        QString botUserName;
        if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginName, botUserName))
        {
            _AddRequestToQueue(QString("https://api.twitch.tv/kraken/users/%1?client_id=%2").arg(botUserName).arg(_krakenParameters[ClientID].toString()));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_InitializeKnownBotStatus()
{
    // KnownBotStatus
    if (_krakenParameters.contains(BotUserID) && !_krakenParameters.contains(KnownBotStatus))
    {
        // Only if we have bot user name and bot user id try to get known bot status
        QString botUserName;
        if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginName, botUserName))
        {
            _AddRequestToQueue(QString("https://api.twitch.tv/kraken/users/%1/chat?api_version=5&client_id=%2").arg(_krakenParameters[BotUserID].toString()).arg(botUserName));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_InitializeChannelUserID()
{
    // ChannelUserID param
    if (!_krakenParameters.contains(ChannelUserID))
    {
        // Only if we have channel user name, try to get channel user id
        QString userName;
        if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginChannel, userName))
        {
            _AddRequestToQueue(QString("https://api.twitch.tv/kraken/users/%1?client_id=%2").arg(userName).arg(_krakenParameters[ClientID].toString()));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_UpdateChannelInfo()
{
    if (_krakenParameters.contains(ChannelUserID))
    {
        if (QDateTime::currentDateTime().toSecsSinceEpoch() % 30 == 0)
        {
            _AddRequestToQueue(QString("https://api.twitch.tv/kraken/channels/%1?api_version=5&client_id=%2").arg(_krakenParameters[ChannelUserID].toString()).arg(_krakenParameters[ClientID].toString()));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_SetParameter(KrakenParameter param, const QVariant& value)
{
    QVariant oldValue = _krakenParameters[param];
    _krakenParameters.insert(param, value);

    if (oldValue != value)
    {
        emit ParameterChanged(param, value);
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_AddRequestToQueue(const QString& request)
{
    _requests.enqueue(request);
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_SendRequest(const QString &request) const
{
    if (_networkManager->networkAccessible() == QNetworkAccessManager::Accessible)
    {
        _networkManager->get(QNetworkRequest(QUrl(request)));

        // Log info
        LOG(LogInfo, QString("Request to Kraken API: %1").arg(request));
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_HandleErrorResponse(const KrakenResponse &response) const
{
    // Log error
    LOG(LogError, QString("Error in request. Response error: %1.\n"
                          "Status: %2.\n"
                          "Message: %3.").arg(response.GetParam("error").toString()).arg(response.GetParam("status").toString()).arg(response.GetParam("message").toString()));
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_HandleUserInforesponse(const KrakenResponse &response)
{
    QString value;
    // Get only user id from request if it is bot user 
    if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginName, value))
    {
        if (value == response.GetParam("name").toString())
        {
            _SetParameter(KrakenParameter::BotUserID, response.GetParam("_id").toUInt());
        }
        else if (ConfigurationManager::Instance().GetStringParam(CfgParam::LoginChannel, value))
        {
            if (value == response.GetParam("name").toString())
            {
                _SetParameter(KrakenParameter::ChannelUserID, response.GetParam("_id").toUInt());
            }
            // If it is another user
            else
            {

            }
        }        
    }
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_HandleKnownBotStatus(const KrakenResponse &response)
{
    // Simply add known bot status to params
    _SetParameter(KrakenParameter::KnownBotStatus, response.GetParam("is_known_bot").toBool());
}

///////////////////////////////////////////////////////////////////////////

void KrakenClient::_HandleChannelInfo(const KrakenResponse& response)
{{
    _SetParameter(KrakenParameter::ChannelTitle, response.GetParam("status").toString());
    _SetParameter(KrakenParameter::ChannelGame, response.GetParam("game").toString());
    _SetParameter(KrakenParameter::ChannelPartnerStatus, response.GetParam("partner").toBool());
    _SetParameter(KrakenParameter::ChannelViews, response.GetParam("views").toUInt());
    _SetParameter(KrakenParameter::ChannelFollowers, response.GetParam("followers").toUInt());
    _SetParameter(KrakenParameter::ChannelBroadcasterType, response.GetParam("broadcaster_type").toString());
}}

///////////////////////////////////////////////////////////////////////////
