#pragma once
#include <QHash>
#include <QString>
#include <QXmlStreamReader>

class ConfigurationManager
{
private:
    QHash<QString, QString> _params;

public:
    static ConfigurationManager& Instance();

    QString Initialize();
    bool GetStringParam(QString parameter, QString& value);

private:
    QXmlStreamReader _xmlReader;

    void _ReadLoginData();
    void _ReadConfigData();
    void _ReadIgnoreList();
    void _ReadCovenantList();

    ConfigurationManager() {}
    ConfigurationManager(ConfigurationManager const&);
    void operator=(ConfigurationManager const&);
};
