#pragma once
#include <QHash>
#include <QString>

class ConfigurationManager
{
private:
    QHash<QString, QString> _params;

public:
    static ConfigurationManager& Instance();

    bool Initialize();
    bool GetStringParam(QString parameter, QString& value);

private:
    ConfigurationManager() {}
    ConfigurationManager(ConfigurationManager const&);
    void operator=(ConfigurationManager const&);
};
