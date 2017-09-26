/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QVector>
#include <QPair>
#include <QJsonObject>

/*!
 * \brief Contains all thing related to Twitch stuff.
 */
namespace Twitch
{  

/*!
 * \brief Simple enumeration to determine type of response.
 */
enum class KrakenResponseType
{
    Undefined,
    Error,
    UserInfo,
    BotStatus
};

/*! Simple typedef to short name. */
typedef QVector<QPair<QString, QVariant> > KRParams;

/*!
 * \brief Class that parses responses and stores params from it then returns type of response.
 */
class KrakenResponse
{
public:
    KrakenResponse();
    /*!
     * \brief Parses response and stores params.
     * \param response - text of response to parse.
     * \return Type of response that was obtained.
     */
    KrakenResponseType ParseResponse(const QString &response);
    /*!
     * \brief Returns all params.
     * \return Array of all parameters.
     */
    const KRParams &GetParams() const;
    /*!
     * \brief Returns specified parameter by name.
     * \param paramName - name of parameter that should be returned.
     * \return Value of specified parameter or empty QVariant.
     */
    const QVariant &GetParam(const QString &paramName) const;

private:
    /*!
     * \brief Checks if specified object contains certain params.
     * \param paramsToValidate - params that should exist on specified object.
     * \param jsonObject - object that should be checked.
     * \return True if specified object have certain parameters. Otherwise - false.
     * 
     * Checks of specified object contains all certain parameters and saves them if it so.
     */
    bool _ValidateParams(const QVector<QString> &paramsToValidate, const QJsonObject &jsonObject);
    /*!
     * \brief Check if specified object contains error fields.
     * \param jsonObject - object that should be checked.
     * \return True if specified object contains error fields.
     */
    bool _ValidateError(const QJsonObject &jsonObject);
    /*!
     * \brief Check if specified object contains user info fields.
     * \param jsonObject - object that should be checked.
     * \return True if specified object contains user info fields.
     */
    bool _ValidateUserInfo(const QJsonObject &jsonObject);
    /*!
     * \brief Check if specified object contains bot status fields.
     * \param jsonObject - object that should be checked.
     * \return True if specified object contains bot status fields.
     */
    bool _ValidateBotStatus(const QJsonObject &jsonObject);

    /*! Array of parameters that was found in response. */
    KRParams _params;

    /*! Static array of fields that should exist in error response. */
    static QVector<QString> _jsonFieldsError;
    /*! Static array of fields that should exist in user info response. */
    static QVector<QString> _jsonFieldsUserInfo;
    /*! Static array of fields that should exist in bot status response. */
    static QVector<QString> _jsonFieldsBotStatus;
};

}