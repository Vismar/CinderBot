/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QObject>
#include <QStringList>
#include <QVector>
#include <QTime>

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

struct CmdParams
{
    QTime Cooldown = QTime(0, 0, 0);
    bool ModeratorOnly = false;
    int Price = 0;
    QString Covenant = "Viewer";
    bool WorkInWhisper = false;
    bool WorkInChat = true;
};

class CustomCommandDBHelper : public QObject
{
    Q_OBJECT
public:
    static CustomCommandDBHelper &Instance();

    QString InititalizeTables();

    /*** Commands ***/
    bool CommandExist(const QString &cmdName);
    bool CreateCommand(CmdType cmdType, const QString &cmdName, const CmdParams &cmdParams);
    bool DeleteCommand(CmdType cmdType, const QString &cmdName);
    QVector<int> GetCommandIds(CmdType cmdType, const QString &covenant = "");
    QStringList GetCommandNames(CmdType cmdType, const QString &covenant = "");

    /*** Command parameters ***/
    CmdParams GetAllParams(CmdType cmdType, int id);
    CmdParams GetAllParams(CmdType cmdType, const QString &cmdName);
    QString GetParameter(CmdType cmdType, const QString &cmdName, CustomCmdParameter cmdParam);
    void SetParameter(CmdType cmdType, const QString &cmdName, CustomCmdParameter cmdParam, const QString &value);

    /*** Command answers ***/
    QVector<int> GetAnswers(CmdType cmdType, const QString &cmdName);
    QString GetRandomAnswer(CmdType cmdType, const QString &cmdName);
    QString GetAnswer(CmdType cmdType, int id);
    void AddAnswer(CmdType cmdType, const QString &cmdName, const QString &answer);
    void EditAnswer(CmdType cmdType, int id, const QString &answer);
    void DeleteAnswer(CmdType cmdType, int id);

signals:
    /*** Commands ***/
    void CustomCmdAdded(CmdType cmdType, const QString &cmdName, int id);
    void CustomCmdDeleted(CmdType cmdType, const QString &cmdName, int id);

    /*** Commands created by user ***/
    void CustomCmdParameterChanged(const QString &cmdName, CustomCmdParameter cmdParam, const QString &value);
    void CustomCmdAnswerAdded(const QString &cmdName);
    void CustomCmdAnswerEdit(const QString &cmdName, int id);
    void CustomCmdAnswerDeleted(const QString &cmdName, int id);

    /*** Commands created by covenants ***/
    void CustomCovCmdParameterChanged(const QString &cmdName, CustomCmdParameter cmdParam, const QString &value);
    void CustomCovCmdAnswerAdded(const QString &cmdName);
    void CustomCovCmdAnswerEdit(const QString &cmdName, int id);
    void CustomCovCmdAnswerDeleted(const QString &cmdName, int id);

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
