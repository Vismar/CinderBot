/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "Logger.hpp"
#include <QTextStream>
#include <QDir>
#include <QDateTime>

using namespace Utils;

///////////////////////////////////////////////////////////////////////////

/* Static variables initialization */
LogLevel Logger::_logLevel = LogLevel::LogWarning;
unsigned char Logger::_maxNumberOfEntries = 1;
QVector<QString> Logger::_logEntries;
int Logger::_logEntryId = 0;
std::mutex Logger::_logMutex;

///////////////////////////////////////////////////////////////////////////

void Logger::SetLogLevel(const LogLevel logLevel)
{
    // Lock data for thread safe execution
    std::lock_guard<std::mutex> lock(_logMutex);
    // Set new log level
    _logLevel = logLevel;
}

///////////////////////////////////////////////////////////////////////////

LogLevel Logger::GetLogLevel()
{
    return _logLevel;
}

///////////////////////////////////////////////////////////////////////////

void Logger::Write(LogLevel logLevel, const QString &message)
{
    // Lock data for thread safe execution
    std::lock_guard<std::mutex> lock(_logMutex);
    // If specified log level is lower or equal to max log level and higher or equal than current log level, create entry
    if ((logLevel >= _logLevel) && (logLevel <=LogCritical))
    {
        // Reserve max possible number (+1) of entries to prevent reallocation
        if (_logEntries.capacity() != _maxNumberOfEntries + 1)
        {
            _logEntries.reserve(_maxNumberOfEntries + 1);
        }

        // Create new entry
        QString entry;
        QTextStream stream(&entry);
        if (_logEntryId == 0)
        {
            stream << "///////////////////////////////////////////////////////////////////////////\n"
                      "///////////////////////////// START OF SESSION ////////////////////////////\n"
                      "///////////////////////////////////////////////////////////////////////////\n\n";
        }
        stream << "Entry id: " << _logEntryId++ << "\n"
                << _GetTimestamp()
                << _GetLogLevel(logLevel)
                << "Message: " << message << "\n\n";
        _logEntries.append(entry);

        // If number of entries is max, write it to file and erase the list
        if (_logEntries.size() == _maxNumberOfEntries)
        {
            _SaveToFile();
            _Erase();
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void Logger::Write(LogLevel logLevel, const QString &className, const QString &funcName, const QString &message)
{
    QString newMessage;
    QTextStream stream(&newMessage);
    stream << "Source ( ";
    if (!className.isEmpty())
    {
        stream << className << "::";
    }
    QString tempFuncName = funcName;
    tempFuncName.replace("__thiscall", "");
    stream << tempFuncName << " )\n" << message;
    Logger::Write(logLevel, newMessage);
}

///////////////////////////////////////////////////////////////////////////

void Logger::Flush()
{
    _SaveToFile();
    _Erase();
}

///////////////////////////////////////////////////////////////////////////

QString Logger::_GetTimestamp()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString timeStamp = QString("Date: %1\nTime: %2\n").arg(dateTime.toString("dd.MM.yyyy")).arg(dateTime.toString("hh:mm:ss"));
    return timeStamp;
}

///////////////////////////////////////////////////////////////////////////

QString Logger::_GetLogLevel(LogLevel logLevel)
{
    QString strLogLevel;
    switch (logLevel)
    {
    case LogDebug:
        strLogLevel = "Type: DEBUG\n";
        break;
    case LogInfo:
        strLogLevel = "Type: INFO\n";
        break;
    case LogWarning:
        strLogLevel = "Type: WARNING\n";
        break;
    case LogError:
        strLogLevel = "Type: ERROR\n";
        break;
    case LogCritical:
        strLogLevel = "Type: CRITICAL\n";
        break;
    default:
        break;
    }

    return strLogLevel;
}

///////////////////////////////////////////////////////////////////////////

void Logger::_SaveToFile()
{
    if (!_logEntries.empty())
    {
        // Check directory
        QDir dir("data/log");
        if (!dir.exists())
        {
            dir.mkpath(".");
        }
        if (dir.exists())
        {
            // Append entries to the end of file
            QFile logFile("./data/log/CinderBot.log");
            if (logFile.open(QIODevice::Append))
            {
                QTextStream logStream(&logFile);
                // Write every entry to file
                for (auto iter = _logEntries.begin(); iter != _logEntries.end(); ++iter)
                {
                    logStream << *iter;
                }
                logFile.close();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void Logger::_Erase()
{
    _logEntries.clear();
}

///////////////////////////////////////////////////////////////////////////
