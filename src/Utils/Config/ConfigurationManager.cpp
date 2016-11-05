#include "ConfigurationManager.hpp"
#include "ConfigurationParameters.hpp"
#include <QFile>
#include <QTextStream>

#define CONFIG_READER_FILE_PATH "./data/config/"
#define CONFIG_READER_FILE_NAME "config.cfg"
#define CONFIG_READER_COMMENT '#'
#define CONFIG_READER_SEPARATOR ' '

ConfigurationManager& ConfigurationManager::Instance()
{
    static ConfigurationManager instance;
    return instance;
}

bool ConfigurationManager::Initialize()
{
    bool result(true);
    QString line;
    QString fileName = CONFIG_READER_FILE_PATH;
    fileName.append(CONFIG_READER_FILE_NAME);
    QFile configFile(fileName);
    if (configFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&configFile);
        while (!in.atEnd())
        {
            line = in.readLine();
            if (line.at(0) != CONFIG_READER_COMMENT)
            {
                int separator = line.indexOf(CONFIG_READER_SEPARATOR);
                if (separator != -1)
                {
                    _params.insert(line.left(separator),
                                   line.right(line.length() - (separator + 1)));
                }
            }
        }
    }
    else
    {
        result = false;
    }
    configFile.close();
    if (!_params.contains(CONFIG_LOGIN_NAME) &&
        !_params.contains(CONFIG_LOGIN_OATH_KEY) &&
        !_params.contains(CONFIG_LOGIN_CHANNEL))
    {
        result = false;
    }

    return result;
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
