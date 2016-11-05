#include "UserData.hpp"
#include <QDebug>
#include <QFile>
#include <QTextStream>

#define USER_DATA_FILE_PATH "./data/data/"
#define USER_DATA_FILE_NAME "userData.data"
#define USER_DATA_PARSER_SS '['
#define USER_DATA_PARSER_ES ']'

void UserData::_SetUserData(const QString& userName,
                            const QString& rawLine)
{
    int startPos(-1), endPos(-1);
    startPos = rawLine.indexOf(USER_DATA_PARSER_SS);
    QHash<QString, QString> params;
    if (startPos != -1)
    {
        endPos = rawLine.indexOf(USER_DATA_PARSER_ES, startPos);
        while ((startPos != -1) && (endPos != -1))
        {
            QString parameter;
            parameter = rawLine.mid(startPos + 1, endPos - startPos - 1);
            int index = parameter.indexOf(',');
            params.insert(parameter.left(index),
                          parameter.right(parameter.length() - index - 1));
            startPos = rawLine.indexOf(USER_DATA_PARSER_SS, endPos);
            endPos = rawLine.indexOf(USER_DATA_PARSER_ES, startPos);
        }
        if (params.size() != _defaultParams.size())
        {
            _SetDefaultUserData(userName, params);
        }
        else
        {
            _AddUserData(userName, params);
        }
    }
}

void UserData::_SetDefaultUserData(const QString& userName,
                                   const QHash<QString, QString>& params)
{
    QHash<QString, QString> newParams;
    for (auto it = _defaultParams.begin(); it != _defaultParams.end(); ++ it)
    {
        if (params.contains(it.key()))
        {
            newParams.insert(it.key(), params.value(it.key()));
        }
        else
        {
            newParams.insert(it.key(), it.value());
        }
    }
    _AddUserData(userName, newParams);
}

void UserData::_ResetToDefaultUserData(const QString& userName)
{
    _AddUserData(userName, _defaultParams);
}

void UserData::_AddUserData(const QString& userName,
                            const QHash<QString, QString>& params)
{
    _userData.insert(userName.toLower(), params);
}

void UserData::_InitializeDefaultUserData()
{
    _defaultParams.insert(_GetUDPParam(UDP_Messages), "0");
    _defaultParams.insert(_GetUDPParam(UDP_Currency), "0");
    _defaultParams.insert(_GetUDPParam(UDP_Covenant), "Viewer");
}

QString UserData::_GetUDPParam(UserDataParam UDP)
{
    QString param;
    switch (UDP)
    {
    case UDP_Messages:
        param = "Messages";
        break;
    case UDP_Currency:
        param = "Currency";
        break;
    case UDP_Covenant:
        param = "Covenant";
        break;
    default:
        param = "NoParam";
        break;
    }

    return param;
}

UserData& UserData::Instance()
{
    static UserData instance;
    return instance;
}

UserData::~UserData()
{
    QString fileName = USER_DATA_FILE_PATH;
    fileName.append(USER_DATA_FILE_NAME);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream in(&file);
        for (auto itKey = _userData.begin(); itKey != _userData.end(); ++itKey)
        {
            in << "@" << itKey.key() << " ";
            for (auto itValue = itKey.value().begin();
                 itValue != itKey.value().end();
                 ++ itValue)
            {
                in << "[" << itValue.key() << "," << itValue.value() << "]";
            }
            in << endl;
        }
    }
}

void UserData::Initialize()
{
    _InitializeDefaultUserData();
    QString line;
    QString fileName = USER_DATA_FILE_PATH;
    fileName.append(USER_DATA_FILE_NAME);
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            line = in.readLine();
            if (line.startsWith('@'))
            {
                int index = line.indexOf(' ');
                if (index != -1)
                {
                    QString userName = line.mid(1, index - 1);
                    QString rawLine = line.right(line.size() - index - 1);
                    _SetUserData(userName, rawLine);
                }
            }
        }
    }
    else
    {
        qDebug() << "No user data file!";
    }
    file.close();
}

QString UserData::GetUserDataParam(const QString& userName, UserDataParam UDP)
{
    if (!_userData.contains(userName.toLower()))
    {
        _AddUserData(userName.toLower(), _defaultParams);
    }
    return _userData[userName.toLower()][_GetUDPParam(UDP)];
}

void UserData::UpdateUserData(const QString& userName,
                              UserDataParam UDP,
                              const QString& newValue)
{
    if (!_userData.contains(userName.toLower()))
    {
        _AddUserData(userName.toLower(), _defaultParams);
    }
    _userData[userName.toLower()].insert(_GetUDPParam(UDP), newValue);
}
