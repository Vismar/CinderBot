/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include <QHash>
#include <QString>
#include <QXmlStreamReader>
#include "ConfigurationParameters.hpp"

/*!
 * \brief Contains all utility classes.
 */
namespace Utils
{

/*!
 * \brief Contains configuration classes.
 */
namespace Configuration
{

/*!
 * \brief Stores configuration parameters and give access to them.
 *
 * Singleton. Handles creating configuration file,
 * reading data from it, storing cfg params, updating it and saving to cfg file.
 */
class ConfigurationManager : public QObject
{
    Q_OBJECT
public:
    ConfigurationManager() : QObject(nullptr) {}
    ConfigurationManager(ConfigurationManager const&) =delete;
    void operator=(ConfigurationManager const&) =delete;

    /*!
     * \brief Makes an instance of manager.
     * \return Static reference to a manager.
     *
     * Creates and returns static instance of ConfigurationManager.
     */
    static ConfigurationManager &Instance();
    /*!
     * \brief Initializes manager, read config file.
     * \return Error string if something goes wrong.
     *
     * Initializes manager:
     * - Checks if needed folders exist and create them if they are not.
     * - Creates config file filled by default values if such file was not found.
     * - Read all config params from xml configuration file.
     */
    QString Initialize();
    /*!
     * \brief Returns config parameter in string.
     * \param cfgParam - parameter that user seek for.
     * \param value - returning value of parameter.
     * \return True if parameter was found.
     *
     * Copies parameter value to specified variable if such parameter exist.
     */
    bool GetStringParam(CfgParam cfgParam, QString &value) const;
    /*!
     * \brief Sets new value to specified parameter, or create new parameter and set specified value to it.
     * \param cfgParam - parameter that user seek for.
     * \param value - value that should be set to specified parameter.
     *
     * Updates specified configuration parameter. Does nothing if the parameter does not exist.
     */
    void SetStringParam(CfgParam cfgParam, const QString &value);

signals:
    /*!
     * \brief Notify subscribers about changing any configuration parameter.
     * \param cfgParam - parameter that was changed.
     *
     * If any parameter was changed during the execution of application,
     * ConfigurationManager will notify all subscribers about change and will send param that was changed.
     */
    void ParameterChanged(CfgParam cfgParam);

private:
    /*!
     * \brief Creates folders if they are not exist.
     * \param error - string which will be used if any kind of error will appear during creation.
     *
     * Checks if needed folders exist and create them if they are not.
     */
    bool _CreateFolders(QString &error) const;

    /*** Read XML data ***/
    /*!
     * \brief Reads login data section from xml file.
     *
     * Reads login params from xml configuration file.
     */
    void _ReadLoginData();
    /*!
     * \brief Reads config data section from xml file.
     *
     * Read all config params from xml configuration file.
     */
    void _ReadConfigData();
    /*!
     * \brief Reads ignore list section from xml file.
     *
     * Reads whole ignore list from xml configuration file.
     */
    void _ReadIgnoreList();
    /*!
     * \brief Read RPG section from xml file.
     * 
     * Reads whole RPG section from xml configuration file that contains multiple sub section for every sub system.
     */
    void _ReadRPG();
    /*!
     * \brief Reads cmd modules section from xml file.
     *
     * Read whole cmd modules section from xml configuration file.
     */
    void _ReadCmdModules();
    /*!
     * \brief Adds all params that do not exist but should.
     *
     * Check all params that should exist and add them if they are not exist in table.
     */
    void _AddParamsThatDoNotExist();

    /*** Write XML data ***/
    /*!
     * \brief Saves all configuration parameters to xml file.
     *
     * Writes all configuration params to xml configuration file.
     */
    void _SaveConfiguration();
    /*!
     * \brief Writes login data section to xml file.
     *
     * Writes all login params to xml configuration file.
     */
    void _WriteLoginData();
    /*!
     * \brief Writes config data section to xml file.
     *
     * Writes all configuration sections of parameters to xml configuration file.
     */
    void _WriteConfigData();
    /*!
     * \brief Writes config currency data section to xml file.
     *
     * Writes all currency params to xml configuration file.
     */
    void _WriteConfigCurrencyData();
    /*!
     * \brief Writes config covenant data section to xml file.
     *
     * Writes all covenant params to xml configuration file.
     */
    void _WriteConfigCovenantData();
    /*!
     * \brief Writes config analytics data section to xml file.
     *
     * Writes all analytics parameters to xml configuration file.
     */
    void _WriteConfigAnalyticsData();
    /*!
     * \brief Writes ignore list section to xml file.
     *
     * Writes list of users that should be ignored by bot to xml configuration file.
     */
    void _WriteIgnoreList();
    /*!
     * \brief Writes command modules section to xml file.
     *
     * Writes command modules data to xml configuration file.
     */
    void _WriteModulesData();

    /*! Hash table of parameters */
    QHash<QString, QString> _params;

    /*! Xml stream reader */
    QXmlStreamReader _xmlReader;
    /*! Xml stream writer */
    QXmlStreamWriter _xmlWriter;
};

}
}
