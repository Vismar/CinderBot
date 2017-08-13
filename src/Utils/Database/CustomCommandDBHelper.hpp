/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include <QStringList>
#include <QVector>

/*!
 * \brief Contains all utility classes.
 */
namespace Utils
{
/*!
 * \brief Contains all database related classes.
 */
namespace Database
{

enum class CmdType
{
    StreamerCmd,
    CovenantCmd
};

enum class CustomCmdParameter
{
    Id = 0,
    Name,
    Cooldown,
    ModeratorOnly,
    Price,
    Covenant,
    WorkInWhisper,
    WorkInChat
};

class CustomCommandDBHelper : public QObject
{
    Q_OBJECT
public:
    static CustomCommandDBHelper &Instance();

    QString InititalizeTables();

    QString GetParameter(CmdType cmdType, const QString &cmdName, CustomCmdParameter cmdParam);
    void SetParameter(CmdType cmdType, const QString &cmdName, CustomCmdParameter cmdParam, const QString &value);
    QVector<int> GetAnswers(CmdType cmdType, const QString &cmdName);
    QString GetRandomAnswer(CmdType cmdType, const QString &cmdName);
    QString GetAnswer(CmdType cmdType, int id);
    void AddAnswer(CmdType cmdType, const QString &cmdName, const QString &answer);
    void EditAnswer(CmdType cmdType, int id, const QString &answer);
    void DeleteAnswer(CmdType cmdType, int id);

signals:
    void CustomCmdParameterChanged(QString cmdName, CustomCmdParameter cmdParam, QString value);
    void CustomCmdAnswerAdded(QString cmdName);
    void CustomCmdAnswerEdit(QString cmdName, int id);
    void CustomCmdAnswerDeleted(QString cmdName, int id);

    void CustomCovCmdParameterChanged(QString cmdName, CustomCmdParameter cmdParam, QString value);
    void CustomCovCmdAnswerAdded(QString cmdName);
    void CustomCovCmdAnswerEdit(QString cmdName, int id);
    void CustomCovCmdAnswerDeleted(QString cmdName, int id);

private:
    enum class TableType
    {
        Commands,
        Answers
    };
    QString _GetTableName(TableType tableType, CmdType cmdType);

    QString _InititalizeCustomCommandTables(const QString &mainTableName, const QString &indexPrefix);
};

}
}
