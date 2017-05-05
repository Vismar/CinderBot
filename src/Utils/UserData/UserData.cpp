#include "UserData.hpp"
#include <QFile>

#define UD_FILE_NAME         "./data/data/UserData.xml"
#define UD_SECTION_USER_DATA "UserData"
#define UD_SECTION_NAME      "Name"

///////////////////////////////////////////////////////////////////////////

UserData& UserData::Instance()
{
    static UserData instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////

UserData::~UserData()
{
    QFile userDataFile(UD_FILE_NAME);
    if (userDataFile.open(QIODevice::WriteOnly))
    {
        _xmlWriter.setDevice(&userDataFile);
        _xmlWriter.setAutoFormatting(true);
        _xmlWriter.writeStartDocument();
        _xmlWriter.writeStartElement("DataTable");
        _SaveUserData();
        _xmlWriter.writeEndElement();
        _xmlWriter.writeEndDocument();
    }
    userDataFile.close();
}

///////////////////////////////////////////////////////////////////////////

void UserData::Initialize()
{
    _InitializeDefaultUserData();
    QFile userDataFile(UD_FILE_NAME);
    if (userDataFile.open(QIODevice::ReadOnly))
    {
        _xmlReader.setDevice(&userDataFile);

        while (!_xmlReader.atEnd())
        {
            _xmlReader.readNext();
            if (_xmlReader.isStartElement())
            {
                if (_xmlReader.name() == UD_SECTION_USER_DATA)
                {
                    _ReadUserData();
                }
            }
        }
    }
    userDataFile.close();
}

///////////////////////////////////////////////////////////////////////////

QString UserData::GetUserDataParam(const QString& userName, UserDataParam UDP)
{
    // If user do not exist, add him to the hash table
    if (!_userData.contains(userName.toLower()))
    {
        _AddUserData(userName.toLower(), _defaultParams);
    }
    return _userData[userName.toLower()][_GetUDPParam(UDP)];
}

///////////////////////////////////////////////////////////////////////////

void UserData::UpdateUserData(const QString& userName,
                              UserDataParam UDP,
                              const QString& newValue)
{
    // If user do not exist, add him to the hash table
    if (!_userData.contains(userName.toLower()))
    {
        _AddUserData(userName.toLower(), _defaultParams);
    }
    _userData[userName.toLower()].insert(_GetUDPParam(UDP), newValue);
}

///////////////////////////////////////////////////////////////////////////

void UserData::_AddUserData(const QString& userName, const QHash<QString, QString>& params)
{
    _userData.insert(userName.toLower(), params);
}

///////////////////////////////////////////////////////////////////////////

void UserData::_ResetToDefaultUserData(const QString& userName)
{
    _AddUserData(userName, _defaultParams);
}

///////////////////////////////////////////////////////////////////////////

void UserData::_InitializeDefaultUserData()
{
    /* Create default values of UDP */
    _defaultParams.insert(_GetUDPParam(UDP_Messages), "0");
    _defaultParams.insert(_GetUDPParam(UDP_Currency), "0");
    _defaultParams.insert(_GetUDPParam(UDP_Covenant), "Viewer");
}

///////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////

void UserData::_ReadUserData()
{
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            // If we reach end of user data section, break the loop
            if (_xmlReader.name() == UD_SECTION_USER_DATA)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            if (_xmlReader.name() == UD_SECTION_NAME)
            {
                QString name = _xmlReader.readElementText().toLower();
                _userData.insert(name, _ReadUserParams());
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void UserData::_SaveUserData()
{
    for (auto it = _userData.begin(); it != _userData.end(); ++it)
    {
        _xmlWriter.writeStartElement(UD_SECTION_USER_DATA);
        _xmlWriter.writeTextElement(UD_SECTION_NAME, it.key());
        for (auto itParam = it.value().begin(); itParam != it.value().end(); ++itParam)
        {
            _xmlWriter.writeTextElement(itParam.key(), itParam.value());
        }
        _xmlWriter.writeEndElement();
    }
}

///////////////////////////////////////////////////////////////////////////

QHash<QString, QString> UserData::_ReadUserParams()
{
    QHash<QString, QString> params;
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            // If we reach end of user data section, break the loop
            if (_xmlReader.name() == UD_SECTION_USER_DATA)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            QString param;
            for (int i = 0; i < UDP_End; ++i)
            {
                param = _GetUDPParam(static_cast<UserDataParam>(i));
                // If need parameter was found, save it
                if (_xmlReader.name() == param)
                {
                    params.insert(param, _xmlReader.readElementText());
                }
            }
        }
    }
    if (params.size() != _defaultParams.size())
    {
        // Check if all UDP were found. If not, add it and set to default value
        for (auto it = _defaultParams.begin(); it != _defaultParams.end(); ++it)
        {
            if (!params.contains(it.key()) || params[it.key()] != "")
            {
                params.insert(it.key(), it.value());
            }
        }
    }

    return params;
}

///////////////////////////////////////////////////////////////////////////
