/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ConfigurationManager.hpp"
#include "Utils/Logger.hpp"
#include <QFile>
#include <QDir>
#include <QVector>
#include <QTextStream>

using namespace Utils::Configuration;

/*! Statis constant array of parameters that could be stored in config manager. */
static const QVector<QString>
CfgStrParam = { // Login params
                "LoginName",
                "LoginDisplayName",
                "LoginNameColor",
                "LoginOauthKey",
                "LoginChannel",
                "LoginChannelOauthKey",
                "LoginAuto",
                // Currency params
                "Currency",
                "CurrencyPerMsg",
                "CurrencyOverTime",
                "CurrencyTimer",
                // Covenant params
                "CovJoinPrice",
                "CovCreatePrice",
                "CovRenamePrice",
                // Analytics params
                "ViewerGraphUpdateTime",
                "MessageGraphUpdateTime",
                // Additional params
                "IgnoreList"
              };

// Section names
#define CFGS_CONFIG_ROOT "Configuration"
#define CFGS_LOGIN       "LoginData"
#define CFGS_CONFIG      "ConfigData"
#define CFGS_IGNORE      "IgnoreList"
#define CFGS_USER        "User"
#define CFGS_COVENANT    "Covenant"

// Folders and files
#define CFG_FOLDER    "data/config"
#define CFG_FILE_NAME "Сonfig.xml"

///////////////////////////////////////////////////////////////////////////

ConfigurationManager &ConfigurationManager::Instance()
{
    static ConfigurationManager instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString ConfigurationManager::Initialize()
{
    QString error;
    // Check if folders are exist
    if (_CreateFolders(error))
    {
        // Create default config file
        _CreateDefaultConfigFile();
    }
    // If folders and config file were created, try to read it
    if (error.isEmpty())
    {
        QString cfgFileName = QString("./%1/%2").arg(CFG_FOLDER).arg(CFG_FILE_NAME);
        QFile configFile(cfgFileName);
        // Try to open config file
        if (configFile.open(QIODevice::ReadOnly))
        {
            _xmlReader.setDevice(&configFile);

            while (!_xmlReader.atEnd())
            {
                _xmlReader.readNext();
                if (_xmlReader.isStartElement())
                {
                    // If we find login section
                    if (_xmlReader.name() == CFGS_LOGIN)
                    {
                        _ReadLoginData();
                    }
                    // If found config parameters
                    if (_xmlReader.name() == CFGS_CONFIG)
                    {
                        _ReadConfigData();
                    }
                }
            }
            if (_xmlReader.hasError())
            {
                error = _xmlReader.errorString();
            }
        }
        else
        {
            error = "No config xml file!";
        }
        configFile.close();
    }

    return error;
}

///////////////////////////////////////////////////////////////////////////

bool ConfigurationManager::GetStringParam(CfgParam cfgParam, QString &value)
{
    // If manager contains requested paramter, get it and return true
    bool result = _params.contains(CfgStrParam[cfgParam]);
    if (result)
    {
        value = _params.value(CfgStrParam[cfgParam]);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::SetStringParam(CfgParam cfgParam, const QString &value)
{
    // If manager contains requested parameter, update it
    if (_params.contains(CfgStrParam[cfgParam]))
    {
        _params.insert(CfgStrParam[cfgParam], value);
    }
    _SaveConfiguration();

    emit ParameterChanged(cfgParam);
}

///////////////////////////////////////////////////////////////////////////

bool ConfigurationManager::_CreateFolders(QString &error)
{
    bool result(false);

    QDir dir(CFG_FOLDER);
    if (!dir.exists())
    {
        // Try to create folders
        if (!dir.mkpath("."))
        {
            LOG(Utils::LogCritical, "", Q_FUNC_INFO, QString("Folder %1 cannot be created.").arg(CFG_FOLDER));
            error = "Folder cannot be created! Please run app as administrator.";
        }
        else
        {
            result = true;
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_CreateDefaultConfigFile()
{
    QFile cfgFile(QString("./%1/%2").arg(CFG_FOLDER).arg(CFG_FILE_NAME));
    if (!cfgFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        LOG(Utils::LogCritical, "", Q_FUNC_INFO, "Config file cannot be created.");
    }
    else
    {
        QTextStream stream(&cfgFile);
        stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                  "<Configuration>\n"
                  "\t<LoginData>\n"
                  "\t\t<LoginName></LoginName>\n"
                  "\t\t<LoginDisplayName></LoginDisplayName>\n"
                  "\t\t<LoginNameColor></LoginNameColor>\n"
                  "\t\t<LoginOauthKey></LoginOauthKey>\n"
                  "\t\t<LoginChannel></LoginChannel>\n"
                  "\t\t<LoginChannelOauthKey></LoginChannelOauthKey>\n"
                  "\t\t<LoginAuto>false</LoginAuto>\n"
                  "\t</LoginData>\n"
                  "\t<ConfigData>\n"
                  "\t\t<IgnoreList>\n"
                  "\t\t\t<User>nightbot</User>\n"
                  "\t\t\t<User>moobot</User>\n"
                  "\t\t\t<User>system message</User>\n"
                  "\t\t</IgnoreList>\n"
                  "\t\t<Currency>Kappa</Currency>\n"
                  "\t\t<CurrencyPerMsg>1</CurrencyPerMsg>\n"
                  "\t\t<CurrencyOverTime>1</CurrencyOverTime>\n"
                  "\t\t<CurrencyTimer>60000</CurrencyTimer>\n"
                  "\t\t<CovJoinPrice>100</CovJoinPrice>\n"
                  "\t\t<CovCreatePrice>2000</CovCreatePrice>\n"
                  "\t\t<CovRenamePrice>500</CovRenamePrice>\n"
                  "\t\t<ViewerGraphUpdateTime>10000</ViewerGraphUpdateTime>\n"
                  "\t\t<MessageGraphUpdateTime>60000</MessageGraphUpdateTime>\n"
                  "\t</ConfigData>\n"
                  "</Configuration>\n";
    }
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_ReadLoginData()
{
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            // If it is end of login section, break the loop
            if (_xmlReader.name() == CFGS_LOGIN)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            // Add parameter to hash table
            _params.insert(_xmlReader.name().toString(), _xmlReader.readElementText());
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_ReadConfigData()
{
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            // If we reach end of configuration section, break the loop
            if (_xmlReader.name() == CFGS_CONFIG)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            // If we found ignore list section, read it
            if (_xmlReader.name() == CFGS_IGNORE)
            {
                _ReadIgnoreList();
            }
            // If we found any other parameter, save it
            else
            {
                QString value = _xmlReader.readElementText();
                if (!value.isEmpty())
                {
                    _params.insert(_xmlReader.name().toString(), value);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_ReadIgnoreList()
{
    QString ignoreList;
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            // If we reach end of ignore list section, break the loop
            if (_xmlReader.name() == CFGS_IGNORE)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            if (_xmlReader.name() == CFGS_USER)
            {
                // Stick all names in one line to easy access via GetStringParam()
                if (!ignoreList.isEmpty())
                {
                    ignoreList.append(',');
                }
                ignoreList.append(_xmlReader.readElementText());
            }
        }
    }
    _params.insert(CFGS_IGNORE, ignoreList);
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_SaveConfiguration()
{
    QString error;
    // Check if folders are exist
    _CreateFolders(error);
    // If folders exist, try to write to file
    if (error.isEmpty())
    {
        QString cfgFileName = QString("./%1/%2").arg(CFG_FOLDER).arg(CFG_FILE_NAME);
        QFile configFile(cfgFileName);
        // Try to open config file
        if (configFile.open(QIODevice::WriteOnly))
        {
            _xmlWriter.setDevice(&configFile);
            _xmlWriter.setAutoFormatting(true);
            _xmlWriter.writeStartDocument();

            _xmlWriter.writeStartElement(CFGS_CONFIG_ROOT);
            _WriteLoginData();            // Login section
            _WriteConfigData();           // Config section
            _xmlWriter.writeEndElement(); // Configuration section end

            _xmlWriter.writeEndDocument();
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteLoginData()
{
    QString value;
    _xmlWriter.writeStartElement(CFGS_LOGIN);

    // LoginName
    _xmlWriter.writeComment("Login name of your bot");
    GetStringParam(LoginName, value);
    _xmlWriter.writeTextElement(CfgStrParam[LoginName], value);
    value.clear();
    // LoginDisplayName
    _xmlWriter.writeComment("Login display name of your bot");
    GetStringParam(LoginDisplayName, value);
    _xmlWriter.writeTextElement(CfgStrParam[LoginDisplayName], value);
    value.clear();
    // LoginNameColor
    _xmlWriter.writeComment("Login name color of your bot");
    GetStringParam(LoginNameColor, value);
    _xmlWriter.writeTextElement(CfgStrParam[LoginNameColor], value);
    value.clear();
    // LoginOauthKey
    _xmlWriter.writeComment("Oauth key which required to login on twitch");
    GetStringParam(LoginOauthKey, value);
    _xmlWriter.writeTextElement(CfgStrParam[LoginOauthKey], value);
    value.clear();
    // LoginChannel
    _xmlWriter.writeComment("Channel to connect");
    GetStringParam(LoginChannel, value);
    _xmlWriter.writeTextElement(CfgStrParam[LoginChannel], value);
    // LoginChannelOauthKey
    _xmlWriter.writeComment("Channel oauth key which required to get stream info");
    GetStringParam(LoginChannelOauthKey, value);
    _xmlWriter.writeTextElement(CfgStrParam[LoginChannelOauthKey], value);
    // LoginAuto
    _xmlWriter.writeComment("Should bot try to connect automatically?");
    GetStringParam(LoginAuto, value);
    _xmlWriter.writeTextElement(CfgStrParam[LoginAuto], value);

    _xmlWriter.writeEndElement(); // Login section end
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteConfigData()
{
    _xmlWriter.writeStartElement(CFGS_CONFIG);

    // Ignore list
    _WriteIgnoreList();
    // Currency params
    _WriteConfigCurrencyData();
    // Covenant params
    _WriteConfigCovenantData();
    // Analytics params
    _WriteConfigAnalyticsData();

    _xmlWriter.writeEndElement(); // Config section end
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteConfigCurrencyData()
{
    QString value;
    /*** Currency parameters ***/
    // Currency
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Currency\n\t\t"
                            "Description: Use any emoticon or definition which you like.\n\t\t"
                            "             Defines how your currency will be represented for viewers.");
    GetStringParam(Currency, value);
    _xmlWriter.writeTextElement(CfgStrParam[Currency], value);
    value.clear();
    // Currency per msg
    _xmlWriter.writeComment("Parameter: Currency per message\n\t\t"
                            "Description: Set value (integer) which will represent\n\t\t"
                            "             how many currency will be given to user per message.");
    GetStringParam(CurrencyPerMsg, value);
    _xmlWriter.writeTextElement(CfgStrParam[CurrencyPerMsg], value);
    value.clear();
    // Currency over time
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Currency per timer\n\t\t"
                            "Description: Set value (integer) which will represent\n\t\t"
                            "             how many currency will be given to user per specified time.");
    GetStringParam(CurrencyOverTime, value);
    _xmlWriter.writeTextElement(CfgStrParam[CurrencyOverTime], value);
    value.clear();
    // Currency timer
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Currency timer\n\t\t"
                            "Description: Timer, which specifies how often bot will give currency to users in chat.\n\t\t"
                            "             Should be in milliseconds, bigger than 0.");
    GetStringParam(CurrencyTimer, value);
    _xmlWriter.writeTextElement(CfgStrParam[CurrencyTimer], value);
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteConfigCovenantData()
{
    QString value;
    /*** Covenant parameters ***/
    // Covenant join price
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Covenant join price\n\t\t"
                            "Description: Price for joining any covenant.\n\t\t"
                            "             Any integer number, bigger than -1.");
    GetStringParam(CovJoinPrice, value);
    _xmlWriter.writeTextElement(CfgStrParam[CovJoinPrice], value);
    value.clear();
    // Covenant create price
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Covenant create price\n\t\t"
                            "Description: Price for creating covenant.\n\t\t"
                            "             Any integer number, bigger than -1.");
    GetStringParam(CovCreatePrice, value);
    _xmlWriter.writeTextElement(CfgStrParam[CovCreatePrice], value);
    value.clear();
    // Covenant rename price
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Covenant rename price\n\t\t"
                            "Description: Price for renaming covenant.\n\t\t"
                            "             Any integer number, bigger than -1.");
    GetStringParam(CovRenamePrice, value);
    _xmlWriter.writeTextElement(CfgStrParam[CovRenamePrice], value);
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteConfigAnalyticsData()
{
    QString value;
    /*** Analytics parameters ***/
    // Analytics viewer graph update time
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Analytics viewer graph update time\n\t\t"
                            "Description: Timer, which specifies how often viewer graph should be updated.\n\t\t"
                            "             In milliseconds, bigger than 0.");
    GetStringParam(ViewerGraphUpdateTime, value);
    _xmlWriter.writeTextElement(CfgStrParam[ViewerGraphUpdateTime], value);
    value.clear();
    // Analytics message graph update time
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Analytics message graph update time\n\t\t"
                            "Description: Timer, which specifies how often message graph should be updated.\n\t\t"
                            "             In milliseconds, bigger than 0.");
    GetStringParam(MessageGraphUpdateTime, value);
    _xmlWriter.writeTextElement(CfgStrParam[MessageGraphUpdateTime], value);
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteIgnoreList()
{
    QString value;
    _xmlWriter.writeComment("\n\t\t"
                            "Bot will ignore these users.");
    _xmlWriter.writeStartElement(CFGS_IGNORE);

    // Get ignore list and split it to users
    GetStringParam(IgnoreList, value);
    QStringList users = value.split(',');
    for (int i = 0; i < users.count(); ++i)
    {
        // Write every user to ignore list
        _xmlWriter.writeTextElement(CFGS_USER, users[i]);
    }

    _xmlWriter.writeEndElement(); // Ignore list section end
}

///////////////////////////////////////////////////////////////////////////
