#pragma once
#include <QString>
#include <QHash>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

enum UserDataParam
{
    UDP_Messages = 0,
    UDP_Currency,
    UDP_Covenant,
    UDP_End
};

class UserData
{
private:
    QHash<QString, QHash<QString, QString> > _userData;
    QHash<QString, QString>                  _defaultParams;

    QXmlStreamReader _xmlReader;
    QXmlStreamWriter _xmlWriter;

    QString _GetUDPParam(UserDataParam UDP);
    void _AddUserData(const QString& userName, const QHash<QString, QString>& params);
    void _ResetToDefaultUserData(const QString& userName);
    void _InitializeDefaultUserData();

    QHash<QString, QString> _ReadUserParams();
    void _ReadUserData();
    void _WriteUserData();

public:
    static UserData& Instance();
    ~UserData();

    void Initialize();

    QString GetUserDataParam(const QString& userName, UserDataParam UDP);
    void UpdateUserData(const QString& userName, UserDataParam UDP, const QString& newValue);
};
