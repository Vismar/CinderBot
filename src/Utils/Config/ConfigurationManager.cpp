#include "ConfigurationManager.hpp"
#include "ConfigurationParameters.hpp"
#include <QFile>
#include <QTextStream>

#define CFG_FILE_NAME "./data/config/config.xml"

ConfigurationManager& ConfigurationManager::Instance()
{
    static ConfigurationManager instance;
    return instance;
}

QString ConfigurationManager::Initialize()
{
    QString error;
    QFile configFile(CFG_FILE_NAME);
    if (configFile.open(QIODevice::ReadOnly))
    {
        _xmlReader.setDevice(&configFile);

        while (!_xmlReader.atEnd())
        {
            _xmlReader.readNext();
            if (_xmlReader.isStartElement())
            {
                if (_xmlReader.name() == CFGS_LOGIN)
                {
                    _ReadLoginData();
                }
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

    return error;
}

bool ConfigurationManager::GetStringParam(QString parameter, QString& value)
{
    bool result = _params.contains(parameter);
    if (result)
    {
        value = _params.value(parameter);
    }

    return result;
}

void ConfigurationManager::_ReadLoginData()
{
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            if (_xmlReader.name() == CFGS_LOGIN)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            _params.insert(_xmlReader.name().toString(), _xmlReader.readElementText());
        }
    }
}

void ConfigurationManager::_ReadConfigData()
{
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            if (_xmlReader.name() == CFGS_CONFIG)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            if (_xmlReader.name() == CFGS_CURRENCY)
            {
                _params.insert(_xmlReader.name().toString(), _xmlReader.readElementText());
            }
            else if (_xmlReader.name() == CFGS_IGNORE)
            {
                _ReadIgnoreList();
            }
            else if (_xmlReader.name() == CFGS_COVENANTS)
            {
                _ReadCovenantList();
            }
        }
    }
}

void ConfigurationManager::_ReadIgnoreList()
{
    QString ignoreList;
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            if (_xmlReader.name() == CFGS_IGNORE)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            if (_xmlReader.name() == CFGS_USER)
            {
                if (!ignoreList.isEmpty())
                {
                    ignoreList.append(',');
                }
                ignoreList.append(_xmlReader.readElementText());
            }
        }
    }
    _params.insert("IgnoreList", ignoreList);
}

void ConfigurationManager::_ReadCovenantList()
{
    QString covenants;
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            if (_xmlReader.name() == CFGS_COVENANTS)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            if (_xmlReader.name() == CFGS_COVENANT)
            {
                if (!covenants.isEmpty())
                {
                    covenants.append(',');
                }
                covenants.append(_xmlReader.readElementText());
            }
        }
    }
    _params.insert("CovenantList", covenants);
}
