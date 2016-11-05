#pragma once
#include <QString>
#include <QHash>

enum UserDataParam
{
    UDP_Messages,
    UDP_Currency,
    UDP_Covenant,
    UDP_End
};

class UserData
{
private:
    QHash<QString, QHash<QString, QString> > _userData;
    QHash<QString, QString>                  _defaultParams;

    void _SetUserData(const QString& userName,
                      const QString& rawLine);
    void _SetDefaultUserData(const QString& userName,
                             const QHash<QString, QString>& params);
    void _ResetToDefaultUserData(const QString& userName);
    void _AddUserData(const QString& userName,
                      const QHash<QString, QString>& params);
    void _InitializeDefaultUserData();

    QString _GetUDPParam(UserDataParam UDP);

public:
    static UserData& Instance();
    ~UserData();

    void Initialize();

    QString GetUserDataParam(const QString& userName,
                             UserDataParam UDP);
    void UpdateUserData(const QString& userName,
                        UserDataParam UDP,
                        const QString& newValue);
};
