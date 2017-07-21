/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QString>
#include <QVector>
#include <mutex>

/*!
 * \brief Contains all utility classes.
 */
namespace Utils
{

/*!
 * \brief Returns class name.
 */
template<typename T>
const char* GetClassName(T)
{
    return typeid(T).name();
}

/*!
 * \brief Enum that contains all log levels.
 */
enum LogLevel
{
    LogDebug = 0,
    LogInfo,
    LogWarning,
    LogError,
    LogCritical
};

/*!
 * \brief This is a static thread safe class that gives you the ability to log events/warnings/errors and etc to file.
 *
 * Static thread safe class, that can write log entries with different log level to file.
 * Also, this class can write messages with class name and function name.
 */
class Logger
{
public:
    Logger(const Logger&) = delete;
    Logger& operator =(const Logger&) = delete;

    /*!
     * \brief Sets current log level.
     * \param logLevel - log level that will be setted.
     *
     * Sets log level to current session. Can be changed whenever it needed.
     */
    static void SetLogLevel(const LogLevel logLevel);
    /*!
     * \brief Gets current log level.
     * \return current log level.
     *
     * Returns currently setted log level.
     */
    static LogLevel GetLogLevel();
    /*!
     * \brief Write simple message without source.
     * \param logLevel - log level for message.
     * \param message - message to write.
     *
     * Write to log simple message without class name and function name.
     */
    static void Write(LogLevel logLevel, const QString &message);
    /*!
     * \brief Write message with source class and function.
     * \param logLevel - log level for message.
     * \param className - name of class.
     * \param funcName - name of function.
     * \param message - message to write.
     *
     * Write to log complex message with class name (it can be empty) and function name.
     */
    static void Write(LogLevel logLevel, const QString &className, const QString &funcName, const QString &message);
    /*!
     * \brief Flush currently stored log entries to file.
     *
     * Writes all stored log entries to log file.
     */
    static void Flush();

private:
    /*!
     * \brief Gets timestamp.
     * \return timestamp string
     *
     * Returns timestamp as a formatted string.
     */
    static QString _GetTimestamp();
    /*!
     * \brief Gets specified log level as string.
     * \param logLevel - specific log leve.
     * \return log level as string.
     *
     * Returns specified log level as formatted string.
     */
    static QString _GetLogLevel(LogLevel logLevel);
    /*!
     * \brief Saves stored entries to file.
     *
     * Saves at the end of log file all stored log entries.
     */
    static void _SaveToFile();
    /*!
     * \brief Erases all currently stored entries.
     *
     * All entries that were stored will be erased.
     */
    static void _Erase();

    /*! Current log leve. */
    static LogLevel _logLevel;
    /*! Mux number of entries to store before save to file. */
    static unsigned char _maxNumberOfEntries;
    /*! Simple storage of log entries. */
    static QVector<QString> _logEntries;
    /*! Id of latest entry. */
    static int _logEntryId;
    /*! Mutex to thread safe work. */
    static std::mutex _logMutex;
};

}

#define LOG       Utils::Logger::Write
#define LOG_FLUSH Utils::Logger::Flush
