/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "KrakenResponse.hpp"
#include <QJsonDocument>

#include <QDebug>

using namespace Twitch;

///////////////////////////////////////////////////////////////////////////
/**************************** Static variables ***************************/
///////////////////////////////////////////////////////////////////////////

static QVariant EMPTY_VARIANT("");

/* Error */
QVector<QString> KrakenResponse::_jsonFieldsError = {"error", "status", "message"};

/* User info */
QVector<QString> KrakenResponse::_jsonFieldsUserInfo = { "display_name", "_id", "name", "type", "bio", "created_at", "updated_at", "logo"};

/* Bot status */
QVector<QString> KrakenResponse::_jsonFieldsBotStatus = { "_id", "login", "display_name", "color", "is_verified_bot", "is_known_bot"};

///////////////////////////////////////////////////////////////////////////

KrakenResponseType KrakenResponse::ParseResponse(const QString &response)
{
    // Set default value of response type
    KrakenResponseType responseType = KrakenResponseType::Undefined;

    // Get json document from raw response data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response.toUtf8());

    // If obtained json document is object, try to validate data in it
    if (jsonDoc.isObject())
    {
        // Get json object from json document
        QJsonObject jsonObject = jsonDoc.object();

        // Validate data and set response type
        if (_ValidateError(jsonObject))
        {
            responseType = KrakenResponseType::Error;
        }
        else if (_ValidateUserInfo(jsonObject))
        {
            responseType = KrakenResponseType::UserInfo;
        }
        else if (_ValidateBotStatus(jsonObject))
        {
            responseType = KrakenResponseType::BotStatus;
        }
    }

    return responseType;
}

///////////////////////////////////////////////////////////////////////////

const KRParams &KrakenResponse::GetParams() const
{
    return _params;
}

///////////////////////////////////////////////////////////////////////////

const QVariant &KrakenResponse::GetParam(const QString& paramName) const
{
    // Try to find required parameter and return it
    for (int i = 0; i < _params.size(); ++i)
    {
        if (_params[i].first == paramName)
        {
            return _params[i].second;
        }
    }

    // If parameter was not found, return empty QVariant
    return EMPTY_VARIANT;
}

///////////////////////////////////////////////////////////////////////////

bool KrakenResponse::_ValidateParams(const QVector<QString> &paramsToValidate, const QJsonObject &jsonObject)
{
    bool validated(true);

    // Check if specified json object contains all certain parameters
    for (int i = 0; i < paramsToValidate.size(); ++i)
    {
        if (!jsonObject.contains(paramsToValidate[i]))
        {
            validated = false;
        }
    }

    // If all parameters in place, save it
    if (validated)
    {
        for (int i = 0; i < paramsToValidate.size(); ++i)
        {
            _params.push_back(QPair<QString, QVariant>(paramsToValidate[i], jsonObject.value(paramsToValidate[i]).toVariant()));
        }
    }

    return validated;
}

///////////////////////////////////////////////////////////////////////////

bool KrakenResponse::_ValidateError(const QJsonObject &jsonObject)
{
    return _ValidateParams(_jsonFieldsError, jsonObject);
}

///////////////////////////////////////////////////////////////////////////

bool KrakenResponse::_ValidateUserInfo(const QJsonObject &jsonObject)
{
    return _ValidateParams(_jsonFieldsUserInfo, jsonObject);
}

///////////////////////////////////////////////////////////////////////////

bool KrakenResponse::_ValidateBotStatus(const QJsonObject &jsonObject)
{
    return _ValidateParams(_jsonFieldsBotStatus, jsonObject);
}

///////////////////////////////////////////////////////////////////////////
