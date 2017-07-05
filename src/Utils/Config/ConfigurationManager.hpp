/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QHash>
#include <QString>
#include <QXmlStreamReader>

/*!
 * Class ConfigurationManager
 * singleton. Store configuration parameters and give access to it
 */
class ConfigurationManager
{
public:
    ConfigurationManager() {}
    ConfigurationManager(ConfigurationManager const&) =delete;
    void operator=(ConfigurationManager const&) =delete;

    /*!
     * Make an instace of manager
     * \return static reference to a manager
     */
    static ConfigurationManager &Instance();
    /*!
     * Initialize manager, read config file
     * \return error string if something goes wrong
     */
    QString Initialize();
    /*!
     * Return config parameter in string
     * \param(IN) parameter - parameter that user seek for
     * \param(OUT) value - returning value of parameter
     * \return true if parameter was found
     */
    bool GetStringParam(const QString &parameter, QString &value);
    /*!
     * Sets new value to specified parameter, or create new parameter and set specified value to it
     * \param(IN) parameter - parameter that user seek for
     * \param(IN) value - value that should be setted to specified parameter
     */
    void SetStringParam(const QString &parameter, QString &value);

private:
    /*!
     * Create folders if they are not exist
     * \param error - string which will be used if any kind of error will appear during creation
     */
    bool _CreateFolders(QString &error);
    /*!
     * Creating config file that doesn't exist
     */
    void _CreateDefaultConfigFile();
    /*** Read XML data ***/
    /*!
     * Read login data section in xml file
     */
    void _ReadLoginData();
    /*!
     * Read config data section in xml file
     */
    void _ReadConfigData();
    /*!
     * Read ignore list section in xml file
     */
    void _ReadIgnoreList();
    /*** Write XML data ***/
    /*!
     * Save all configuration parameters to xml file
     */
    void _SaveConfiguration();
    /*!
     * Write login data section to xml file
     */
    void _WriteLoginData();
    /*!
     * Write config data section to xml file
     */
    void _WriteConfigData();
    /*!
     * Write config currency data section to xml file
     */
    void _WriteConfigCurrencyData();
    /*!
     * Write config covenant data section to xml file
     */
    void _WriteConfigCovenantData();
    /*!
     * Write config analytics data section to xml file
     */
    void _WriteConfigAnalyticsData();
    /*!
     * Write ignore list section to xml file
     */
    void _WriteIgnoreList();

    /*! Hash table of parameters */
    QHash<QString, QString> _params;

    /*! Xml stream reader */
    QXmlStreamReader _xmlReader;
    /*! Xml stream writer */
    QXmlStreamWriter _xmlWriter;
};
