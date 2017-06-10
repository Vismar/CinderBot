/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ConfigurationManager.hpp"
#include "ConfigurationParameters.hpp"
#include <QFile>

#define CFG_FILE_NAME "./data/config/Сonfig.xml"

///////////////////////////////////////////////////////////////////////////

ConfigurationManager& ConfigurationManager::Instance()
{
    static ConfigurationManager instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

QString ConfigurationManager::Initialize()
{
    QString error;
    QFile configFile(CFG_FILE_NAME);
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

    return error;
}

///////////////////////////////////////////////////////////////////////////

bool ConfigurationManager::GetStringParam(const QString& parameter, QString& value)
{
    // If manager contains requested paramter, get it and return true
    bool result = _params.contains(parameter);
    if (result)
    {
        value = _params.value(parameter);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////

void ConfigurationManager::SetStringParam(const QString& parameter, QString& value)
{
    _params.insert(parameter, value);
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
    _params.insert("IgnoreList", ignoreList);
}

///////////////////////////////////////////////////////////////////////////
