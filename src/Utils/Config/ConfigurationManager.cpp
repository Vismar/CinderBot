/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ConfigurationManager.hpp"
#include "Utils/Logger.hpp"
#include <QDir>
#include <QVector>

#include <QDebug>

using namespace Utils::Configuration;

/*! Static constant array of parameters that could be stored in config manager. */
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
                "CurrencyPerBit",
                // Covenant params
                "CovJoinPrice",
                "CovCreatePrice",
                "CovRenamePrice",
                "CovExpToLvl",
                "CovExpTimer",
                "CovExpGain",
                "CovMembersDefault",
                "CovMembersPerUp",
                "CovMembersLvlRequirement",
                "CovCmdSlotsDefault",
                "CovCmdSlotsPerUp",
                "CovCmdSlotsLvlRequirement",
                // Analytics params
                "ViewerGraphUpdateTime",
                "MessageGraphUpdateTime",
                // Additional params
                "IgnoreList",
                // Inbuilt command modules
                "UserDataCmdModule",
                "UserDataCmdRcvWhisper",
                "UserDataCmdRcvChat",
                "QuotesCmdModule",
                "CovenantCmdModule",
                "CovenantCmdRcvWhisper",
                "CovenantCmdRcvChat",
                "CustomCmdModule"
              };

// Section names
#define CFGS_CONFIG_ROOT   "Configuration"
#define CFGS_LOGIN         "LoginData"
#define CFGS_CONFIG        "ConfigData"
#define CFGS_IGNORE        "IgnoreList"
#define CFGS_USER          "User"
#define CFGS_RPG           "RPG"
#define CFGS_COVENANTS     "Covenants"
#define CFGS_COV_PRICES    "CovenantPrices"
#define CFGS_COV_EXP       "CovenantExp"
#define CFGS_COV_MEMBERS   "CovenantMembers"
#define CFGS_COV_CMD_SLOTS "CovenantCmdSlots"
#define CFGS_CMD_MODULES   "CmdModules"

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
    _CreateFolders(error);

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
            configFile.close();
            if (_xmlReader.hasError())
            {
                error = _xmlReader.errorString();
            }
            _AddParamsThatDoNotExist();
        }
        else
        {
            _AddParamsThatDoNotExist();
        }
    }

    return error;
}

///////////////////////////////////////////////////////////////////////////

bool ConfigurationManager::GetStringParam(CfgParam cfgParam, QString &value) const
{
    // If manager contains requested parameter, get it and return true
    bool result = _params.contains(CfgStrParam[static_cast<int>(cfgParam)]);
    if (result)
    {
        value = _params.value(CfgStrParam[static_cast<int>(cfgParam)]);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::SetStringParam(CfgParam cfgParam, const QString &value)
{
    // If manager contains requested parameter, update it
    if (_params.contains(CfgStrParam[static_cast<int>(cfgParam)]))
    {
        _params.insert(CfgStrParam[static_cast<int>(cfgParam)], value);
    }
    _SaveConfiguration();

    emit ParameterChanged(cfgParam);
}

///////////////////////////////////////////////////////////////////////////

bool ConfigurationManager::_CreateFolders(QString &error) const
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
            else if (_xmlReader.name() == CFGS_RPG)
            {
                _ReadRPG();
            }
            else if (_xmlReader.name() == CFGS_CMD_MODULES)
            {
                _ReadCmdModules();
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

void ConfigurationManager::_ReadRPG()
{
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        // If we reach end of RPG section, break the loop
        if (_xmlReader.isEndElement() && _xmlReader.name() == CFGS_RPG)
        {
            break;
        }
        if (_xmlReader.isStartElement())
        {
            // If it is not start of a sub section, save parameter
            if ((_xmlReader.name() != CFGS_COVENANTS) &&
                (_xmlReader.name() != CFGS_COV_PRICES) &&
                (_xmlReader.name() != CFGS_COV_EXP) &&
                (_xmlReader.name() != CFGS_COV_MEMBERS) &&
                (_xmlReader.name() != CFGS_COV_CMD_SLOTS))
            {
                _params.insert(_xmlReader.name().toString(), _xmlReader.readElementText());
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_ReadCmdModules()
{
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            // If we reach end of cmd modules section, break the loop
            if (_xmlReader.name() == CFGS_CMD_MODULES)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            // Save all parameters
            _params.insert(_xmlReader.name().toString(), _xmlReader.readElementText());
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_AddParamsThatDoNotExist()
{
    bool paramAdded(false);
    QString tempStr;
    CfgParam param;
    QString value;
    for (int i = 0; i < static_cast<int>(CfgParam::EndOfParams); ++i)
    {
        value = "NO VALUE";
        param = static_cast<CfgParam>(i);
        if (!GetStringParam(param, tempStr))
        {
            switch (param)
            {
            // Login params
            {
            case CfgParam::LoginName:
                value.clear();
                break;
            case CfgParam::LoginDisplayName:
                value.clear();
                break;
            case CfgParam::LoginNameColor:
                value = "#000000";
                break;
            case CfgParam::LoginOauthKey:
                value.clear();
                break;
            case CfgParam::LoginChannel:
                value.clear();
                break;
            case CfgParam::LoginChannelOauthKey:
                value.clear();
                break;
            case CfgParam::LoginAuto:
                value = "false";
                break;
            }
            // Currency params
            {
            case CfgParam::Currency:
                value = "Kappa";
                break;
            case CfgParam::CurrencyPerMsg:
                value = "1";
                break;
            case CfgParam::CurrencyOverTime:
                value = "1";
                break;
            case CfgParam::CurrencyTimer:
                value = "60000";
                break;
            case CfgParam::CurrencyPerBit:
                value = "2";
                break;
            }
            // Covenant params
            {
            case CfgParam::CovJoinPrice:
                value = "100";
                break;
            case CfgParam::CovCreatePrice:
                value = "2000";
                break;
            case CfgParam::CovRenamePrice:
                value = "500";
                break;
            case CfgParam::CovExpToLvl:
                value = "5000";
                break;
            case CfgParam::CovExpTimer:
                value = "300000";
                break;
            case CfgParam::CovExpGain:
                value = "10";
                break;
            case CfgParam::CovMembersDefault:
                value = "10";
                break;
            case CfgParam::CovMembersPerUp:
                value = "1";
                break;
            case CfgParam::CovMembersLvlRequirement:
                value = "1";
                break;
            case CfgParam::CovCmdSlotsDefault:
                value = "1";
                break;
            case CfgParam::CovCmdSlotsPerUp:
                value = "1";
                break;
            case CfgParam::CovCmdSlotsLvlRequirement:
                value = "2";
                break;
            }
            // Analytics params
            {
            case CfgParam::ViewerGraphUpdateTime:
                value = "10000";
                break;
            case CfgParam::MessageGraphUpdateTime:
                value = "60000";
                break;
            }
            // Additional params
            {
            case CfgParam::IgnoreList:
                value = "";
                break;
            }
            // Inbuilt command modules
            {
            case CfgParam::UserDataCmdModule:
                value = "true";
                break;
            case CfgParam::UserDataCmdRcvWhisper:
                value = "true";
                break;
            case CfgParam::UserDataCmdRcvChat:
                value = "true";
                break;
            case CfgParam::QuotesCmdModule:
                value = "true";
                break;
            case CfgParam::CovenantCmdModule:
                value = "true";
                break;
            case CfgParam::CovenantCmdRcvWhisper:
                value = "true";
                break;
            case CfgParam::CovenantCmdRcvChat:
                value = "true";
                break;
            case CfgParam::CustomCmdModule:
                value = "true";
                break;
            }
            default:
                break;
            }

            if (value != "NO VALUE")
            {
                paramAdded = true;
                _params.insert(CfgStrParam[i], value);
            }
        }
    }

    if (paramAdded)
    {
        _SaveConfiguration();
    }
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

            configFile.close();
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
    GetStringParam(CfgParam::LoginName, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::LoginName)], value);
    value.clear();

    // LoginDisplayName
    _xmlWriter.writeComment("Login display name of your bot");
    GetStringParam(CfgParam::LoginDisplayName, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::LoginDisplayName)], value);
    value.clear();

    // LoginNameColor
    _xmlWriter.writeComment("Login name color of your bot");
    GetStringParam(CfgParam::LoginNameColor, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::LoginNameColor)], value);
    value.clear();

    // LoginOauthKey
    _xmlWriter.writeComment("Oauth key which required to login on twitch");
    GetStringParam(CfgParam::LoginOauthKey, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::LoginOauthKey)], value);
    value.clear();

    // LoginChannel
    _xmlWriter.writeComment("Channel to connect");
    GetStringParam(CfgParam::LoginChannel, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::LoginChannel)], value);
    value.clear();

    // LoginChannelOauthKey
    _xmlWriter.writeComment("Channel oauth key which required to get stream info");
    GetStringParam(CfgParam::LoginChannelOauthKey, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::LoginChannelOauthKey)], value);
    value.clear();

    // LoginAuto
    _xmlWriter.writeComment("Should bot try to connect automatically?");
    GetStringParam(CfgParam::LoginAuto, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::LoginAuto)], value);

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
    // Command modules params
    _WriteModulesData();

    _xmlWriter.writeEndElement(); // Config section end
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteConfigCurrencyData()
{
    QString value;
    // Currency
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Currency\n\t\t"
                            "Description: Use any emoticon or definition which you like.\n\t\t"
                            "             Defines how your currency will be represented for viewers.");
    GetStringParam(CfgParam::Currency, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::Currency)], value);
    value.clear();

    // Currency per msg
    _xmlWriter.writeComment("Parameter: Currency per message\n\t\t"
                            "Description: Set value (integer) which will represent\n\t\t"
                            "             how many currency will be given to user per message.");
    GetStringParam(CfgParam::CurrencyPerMsg, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CurrencyPerMsg)], value);
    value.clear();

    // Currency over time
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Currency per timer\n\t\t"
                            "Description: Set value (integer) which will represent\n\t\t"
                            "             how many currency will be given to user per specified time.");
    GetStringParam(CfgParam::CurrencyOverTime, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CurrencyOverTime)], value);
    value.clear();

    // Currency timer
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Currency timer\n\t\t"
                            "Description: Timer, which specifies how often bot will give currency to users in chat.\n\t\t"
                            "             Should be in milliseconds, bigger than 0.");
    GetStringParam(CfgParam::CurrencyTimer, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CurrencyTimer)], value);
    value.clear();

    // Currency per bit
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Currency per bit\n\t\t"
                            "Description: Set value (integer) which will represent\n\t\t"
                            "             how many currency will be given to user per every donated bit.");
    GetStringParam(CfgParam::CurrencyPerBit, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CurrencyPerBit)], value);
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteConfigCovenantData()
{
    QString value;

    // RPG section 
    _xmlWriter.writeComment("\n\t\t"
                            "RPG section that contains all parameters for RPG system.");
    _xmlWriter.writeStartElement(CFGS_RPG);

    // Covenants section
    _xmlWriter.writeComment("\n\t\t\t"
                            "Covenants section that contains all covenant parameters for RPG system.");
    _xmlWriter.writeStartElement(CFGS_COVENANTS);

    /****************************/
    /**** Covenant prices ****/
    /****************************/
    // Covenant prices section
    _xmlWriter.writeComment("\n\t\t\t\t"
                            "Covenant prices section that contains all price parameters for covenants.");
    _xmlWriter.writeStartElement(CFGS_COV_PRICES);

    // Covenant join price
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant join price\n\t\t\t\t\t"
                            "Description: Price for joining any covenant.\n\t\t\t\t\t"
                            "             Any integer number, bigger than -1.");
    GetStringParam(CfgParam::CovJoinPrice, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovJoinPrice)], value);
    value.clear();

    // Covenant create price
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant create price\n\t\t\t\t\t"
                            "Description: Price for creating covenant.\n\t\t\t\t\t"
                            "             Any integer number, bigger than -1.");
    GetStringParam(CfgParam::CovCreatePrice, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovCreatePrice)], value);
    value.clear();

    // Covenant rename price
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant rename price\n\t\t\t\t\t"
                            "Description: Price for renaming covenant.\n\t\t\t\t\t"
                            "             Any integer number, bigger than -1.");
    GetStringParam(CfgParam::CovRenamePrice, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovRenamePrice)], value);
    value.clear();

    _xmlWriter.writeEndElement(); // Covenant prices section end

    /****************************/
    /******* Covenant exp *******/
    /****************************/
    _xmlWriter.writeComment("\n\t\t\t\t"
                            "Covenant exp section that contains all exp parameters for covenants.");
    _xmlWriter.writeStartElement(CFGS_COV_EXP);

    // Covenant experience required to lvl up
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant experience required to lvl up\n\t\t\t\t\t"
                            "Description: Amount of experience to lvl up covenant from first level to second.\n\t\t\t\t\t"
                            "             Required number for all other leveles will be calculated as LEVEL * VALUE.\n\t\t\t\t\t"
                            "             Value should be any integer number, bigger than 0.");
    GetStringParam(CfgParam::CovExpToLvl, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovExpToLvl)], value);
    value.clear();

    // Covenant exp gain timer
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant exp gain timer\n\t\t\t\t\t"
                            "Description: Timer, which specified how often covenants will gain exp.\n\t\t\t\t\t"
                            "             In milliseconds, bigger than 0.");
    GetStringParam(CfgParam::CovExpTimer, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovExpTimer)], value);
    value.clear();

    // Covenant exp gain value
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant exp gain value\n\t\t\t\t\t"
                            "Description: Amount of experience that will be give to covenant over time.\n\t\t\t\t\t"
                            "             Final number depends on number of covenant members in chat(VALUE * SQRT4(MEMBERS_IN_CHAT)).\n\t\t\t\t\t"
                            "             Any integer number, bigger than 0.");
    GetStringParam(CfgParam::CovExpGain, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovExpGain)], value);
    value.clear();

    _xmlWriter.writeEndElement(); // Covenant exp section end

    /****************************/
    /** Covenant members slots **/
    /****************************/
    _xmlWriter.writeComment("\n\t\t\t\t"
                            "Covenant members section that contains all members parameters for covenants.");
    _xmlWriter.writeStartElement(CFGS_COV_MEMBERS);

    // Covenant default members number
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant default members number\n\t\t\t\t\t"
                            "Description: Number of members that can be in covenant on level 1.\n\t\t\t\t\t"
                            "             Any integer number, bigger than 0.");
    GetStringParam(CfgParam::CovMembersDefault, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovMembersDefault)], value);
    value.clear();
    
    // Covenant members number per up
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant members number per up\n\t\t\t\t\t"
                            "Description: Number of members slots that will be added per required up.\n\t\t\t\t\t"
                            "             Any integer number.");
    GetStringParam(CfgParam::CovMembersPerUp, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovMembersPerUp)], value);
    value.clear();
    
    // Covenant members level requirement
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant members level requirement\n\t\t\t\t\t"
                            "Description: Every N levels number of members will be increased.\n\t\t\t\t\t"
                            "             Any integer number, bigger than 0.");
    GetStringParam(CfgParam::CovMembersLvlRequirement, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovMembersLvlRequirement)], value);
    value.clear();

    _xmlWriter.writeEndElement(); // Covenant members section end
    
    /****************************/
    /**** Covenant cmd slots ****/
    /****************************/
    _xmlWriter.writeComment("\n\t\t\t\t"
                            "Covenant cmd slots section that contains all cmd slots parameters for covenants.");
    _xmlWriter.writeStartElement(CFGS_COV_CMD_SLOTS);

    // Covenant default command slots number
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant default command slots number\n\t\t\t\t\t"
                            "Description: Number of command slots that is available at level 1 of covenant.\n\t\t\t\t\t"
                            "             Any integer number.");
    GetStringParam(CfgParam::CovCmdSlotsDefault, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovCmdSlotsDefault)], value);
    value.clear();
    
    // Covenant command slots per up
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant command slots per up\n\t\t\t\t\t"
                            "Description: Number of command slots that will be added per required up.\n\t\t\t\t\t"
                            "             Any integer number.");
    GetStringParam(CfgParam::CovCmdSlotsPerUp, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovCmdSlotsPerUp)], value);
    value.clear();
    
    // Covenant command slots requirement
    _xmlWriter.writeComment("\n\t\t\t\t\t"
                            "Parameter: Covenant command slots requirement\n\t\t\t\t\t"
                            "Description: Every N levels number of command slots will increased.\n\t\t\t\t\t"
                            "             Any integer number, bigger than 0.");
    GetStringParam(CfgParam::CovCmdSlotsLvlRequirement, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovCmdSlotsLvlRequirement)], value);

    _xmlWriter.writeEndElement(); // Covenant cmd slots section end

    _xmlWriter.writeEndElement(); // Covenants section end
    _xmlWriter.writeEndElement(); // RPG section end
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteConfigAnalyticsData()
{
    QString value;
    // Analytics viewer graph update time
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Analytics viewer graph update time\n\t\t"
                            "Description: Timer, which specifies how often viewer graph should be updated.\n\t\t"
                            "             In milliseconds, bigger than 0.");
    GetStringParam(CfgParam::ViewerGraphUpdateTime, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::ViewerGraphUpdateTime)], value);
    value.clear();

    // Analytics message graph update time
    _xmlWriter.writeComment("\n\t\t"
                            "Parameter: Analytics message graph update time\n\t\t"
                            "Description: Timer, which specifies how often message graph should be updated.\n\t\t"
                            "             In milliseconds, bigger than 0.");
    GetStringParam(CfgParam::MessageGraphUpdateTime, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::MessageGraphUpdateTime)], value);
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteIgnoreList()
{
    QString value;
    _xmlWriter.writeComment("\n\t\t"
                            "Bot will ignore these users.");
    _xmlWriter.writeStartElement(CFGS_IGNORE);

    // Get ignore list and split it to users
    GetStringParam(CfgParam::IgnoreList, value);
    QStringList users = value.split(',');
    for (int i = 0; i < users.count(); ++i)
    {
        // Write every user to ignore list
        _xmlWriter.writeTextElement(CFGS_USER, users[i]);
    }

    _xmlWriter.writeEndElement(); // Ignore list section end
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::_WriteModulesData()
{
    QString value;
    _xmlWriter.writeStartElement(CFGS_CMD_MODULES);

    /****************************/
    /*** User data cmd module ***/
    /****************************/
    _xmlWriter.writeComment("\n\t\t\t\tUserData CmdModule\n\t\t\t");
    // User data command modules
    _xmlWriter.writeComment("\n\t\t\t"
                            "Parameter: User data command module\n\t\t\t"
                            "Description: Value of this parameter turning on and off commands related to user data.\n\t\t\t"
                            "             Should be set to \"true\" or \"false\"");
    GetStringParam(CfgParam::UserDataCmdModule, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::UserDataCmdModule)], value);
    value.clear();
    // User data command receive whisper
    _xmlWriter.writeComment("\n\t\t\t"
                            "Parameter: User data command receive whisper\n\t\t\t"
                            "Description: Value of this parameter turning on and off execution for UD commands\n\t\t\t"
                            "             through whispers.\n\t\t\t"
                            "             Should be set to \"true\" or \"false\"");
    GetStringParam(CfgParam::UserDataCmdRcvWhisper, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::UserDataCmdRcvWhisper)], value);
    value.clear();
    // User data command receive chat
    _xmlWriter.writeComment("\n\t\t\t"
                            "Parameter: User data command receive chat\n\t\t\t"
                            "Description: Value of this parameter turning on and off execution for UD commands\n\t\t\t"
                            "             through chat.\n\t\t\t"
                            "             Should be set to \"true\" or \"false\"");
    GetStringParam(CfgParam::UserDataCmdRcvChat, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::UserDataCmdRcvChat)], value);
    value.clear();

    /****************************/
    /***** Quote cmd module *****/
    /****************************/
    _xmlWriter.writeComment("\n\t\t\t\tQuote CmdModule\n\t\t\t");
    // Quotes command modules
    _xmlWriter.writeComment("\n\t\t\t"
                            "Parameter: Quotes command module\n\t\t\t"
                            "Description: Value of this parameter turning on and off commands related to quotes.\n\t\t\t"
                            "             Should be set to \"true\" or \"false\"");
    GetStringParam(CfgParam::QuotesCmdModule, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::QuotesCmdModule)], value);
    value.clear();

    /****************************/
    /*** Covenants cmd module ***/
    /****************************/
    _xmlWriter.writeComment("\n\t\t\t\tCovenants CmdModule\n\t\t\t");
    // Covenant command modules
    _xmlWriter.writeComment("\n\t\t\t"
                            "Parameter: Covenant command module\n\t\t\t"
                            "Description: Value of this parameter turning on and off commands related to covenants.\n\t\t\t"
                            "             Should be set to \"true\" or \"false\"");
    GetStringParam(CfgParam::CovenantCmdModule, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovenantCmdModule)], value);
    value.clear();
    // Covenant command receive whisper
    _xmlWriter.writeComment("\n\t\t\t"
                            "Parameter: Covenant command receive whisper\n\t\t\t"
                            "Description: Value of this parameter turning on and off execution for UD commands\n\t\t\t"
                            "             through whispers.\n\t\t\t"
                            "             Should be set to \"true\" or \"false\"");
    GetStringParam(CfgParam::CovenantCmdRcvWhisper, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovenantCmdRcvWhisper)], value);
    value.clear();
    // Covenant command receive chat
    _xmlWriter.writeComment("\n\t\t\t"
                            "Parameter: Covenant command receive chat\n\t\t\t"
                            "Description: Value of this parameter turning on and off execution for UD commands\n\t\t\t"
                            "             through chat.\n\t\t\t"
                            "             Should be set to \"true\" or \"false\"");
    GetStringParam(CfgParam::CovenantCmdRcvChat, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CovenantCmdRcvChat)], value);
    value.clear();

    /****************************/
    /**** Custom cmd module *****/
    /****************************/
    _xmlWriter.writeComment("\n\t\t\t\tCustom CmdModule\n\t\t\t");
    // Custom command modules
    _xmlWriter.writeComment("\n\t\t\t"
                            "Parameter: Custom command module\n\t\t\t"
                            "Description: Value of this parameter turning on and off custom commands.\n\t\t\t"
                            "             Should be set to \"true\" or \"false\"");
    GetStringParam(CfgParam::CustomCmdModule, value);
    _xmlWriter.writeTextElement(CfgStrParam[static_cast<int>(CfgParam::CustomCmdModule)], value);

    _xmlWriter.writeEndElement(); // Cmd modules section end
}

///////////////////////////////////////////////////////////////////////////
