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
private:
    /*! Hash table of parameters */
    QHash<QString, QString> _params;

public:
    /*!
     * Make an instace of manager
     * \return static reference to a manager
     */
    static ConfigurationManager& Instance();
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
    bool GetStringParam(QString parameter, QString& value);

private:
    /*! Xml stream reader */
    QXmlStreamReader _xmlReader;

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
    /*!
     * Read covenant list section in xml file
     */
    void _ReadCovenantList();

    ConfigurationManager() {}
    ConfigurationManager(ConfigurationManager const&);
    void operator=(ConfigurationManager const&);
};
