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

/*!
 * \brief Small enumeration to separate command types.
 */
enum class CmdType
{
    StreamerCmd, /*! Type that should be used for commands that was created by user (aka streamer). */
    CovenantCmd  /*! Type that should be used for commands that was created for covenants by viewers/chatters. */
};

/*!
* \brief Defines all command parameters.
*/
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

/*!
* \brief Small container to store all command parameters except Id and Name.
*/
struct CmdParams
{
    /*!
     * \brief Gets all params as a string which contains name of params and their values.
     */
    QString ToParamString() const;
    /*!
     * \brief Gets all params as a string which contains only values of params.
     */
    QString ToAddString() const;

    /*!
     * \brief Check if values of CmdParams are guarded (for example, if price value is < 0 then it should be set to 0).
     */
    void GuardParams();

    /*! Cooldown */
    QTime Cooldown = QTime(0, 0, 0);
    /*! ModOnly flag */
    bool ModeratorOnly = false;
    /*! Price */
    int Price = 0;
    /*! Covenant */
    QString Covenant = "Viewer";
    /*! Work in whisper flag */
    bool WorkInWhisper = false;
    /*! Work in chat flag */
    bool WorkInChat = true;        
};

/*!
 * \brief Helper class designed to facilitate the work with the database for custom commands.
 * 
 * Handles the creating database tables, provides simple interface to create/update/edit commands and their answers.
 * In addition, emits events about any changes in custom command related database tables.
 */
class CustomCommandDBHelper : public QObject
{
    Q_OBJECT
public:
    static CustomCommandDBHelper &Instance();

    /*!
     * \brief Tries to create all custom command tables.
     * \return "OK" or error text if something goes wrong.
     * 
     * Initializes all tables one by one. If something goes wrong in creation process, will return error. Otherwise - "OK".
     */
    static QString InititalizeTables();

    /*** Commands ***/
    /*!
     * \brief Checks if specified command exist.
     * \param cmdName - Command name which should be checked.
     * \return True if command exist. Otherwise - false.
     */
    static bool CommandExist(const QString &cmdName, const QString &covenant = "");
    /*!
     * \brief Tries to create custom command.
     * \param cmdType - Type of a new custom command.
     * \param cmdName - Name of a new custom command.
     * \param cmdParams - Parameters of a new custom command.
     * \return True if command was created. Otherwise - false.
     */
    bool CreateCommand(CmdType cmdType, const QString &cmdName, CmdParams &cmdParams);
    /*!
     * \brief Tries to delete custom command.
     * \param cmdType - Type of custom command.
     * \param cmdName - Name of custom command.
     * \return True if command was deleted. Otherwise - false.
     */
    bool DeleteCommand(CmdType cmdType, const QString &cmdName);
    /*!
     * \brief Grabs all command ids.
     * \param cmdType - Type of command that should be grabbed.
     * \param covenant - Covenant name that should be searched in all commands.
     * \return Array of command ids.
     *
     * Returns all command ids of specified command type. 
     * If covenant was specified, will return commands only with such value.
     */
    static QVector<int> GetCommandIds(CmdType cmdType, const QString &covenant = "");
    /*!
     * \brief Grabs all command names.
     * \param cmdType - Type of command that should be grabbed.
     * \param covenant - Covenant name that should be searched in all commands.
     * \return Array of command names.
     *
     * Returns all command names of specified command type. 
     * If covenant was specified, will return commands only with such value.
     */
    static QStringList GetCommandNames(CmdType cmdType, const QString &covenant = "");
    /*!
     * \brief Grabs number of custom commands.
     * \param cmdType - Type of required commands.
     * \param covenant - Covenant name that should be searched in all commands.
     * 
     * Returns number of commands that does exist in database.
     */
    static int GetNumberOfCommands(CmdType cmdType, const QString &covenant = "");

    /*** Command parameters ***/
    /*!
     * \brief Gets all parameters of custom command by specified id.
     * \param cmdType - Type of command that should be grabbed.
     * \param id - Id of command that should be searched to return params.
     * \return CmdParams fills by command parameters.
     * 
     * If id not exist then default params will be returned.
     */
    static CmdParams GetAllParams(CmdType cmdType, int id);
    /*!
     * \brief Gets all parameters of custom command by specified name.
     * \param cmdType - Type of command that should be grabbed.
     * \param cmdName - Name of command that should be searched to return params.
     * \return CmdParams fills by command parameters.
     * 
     * If name not exist then default params will be returned.
     */
    static CmdParams GetAllParams(CmdType cmdType, const QString &cmdName);
    /*!
     * \brief Sets all params to specified command.
     * \param cmdType - Type of command that should be updated.
     * \param cmdName - Name of command that should be updated.
     * \param cmdParams - CmdParams filled by new params.
     */
    void SetAllParams(CmdType cmdType, const QString &cmdName, CmdParams &cmdParams);
    /*!
     * \brief Gets specified parameter.
     * \param cmdType - Type of command that should be returned.
     * \param cmdName - Name of command that should be returned.
     * \param cmdParam - CustomCmdParameter that should be returned.
     * \return Value of specified parameter. Will be empty if something goes wrong.
     */
    static QString GetParameter(CmdType cmdType, const QString &cmdName, CustomCmdParameter cmdParam);
    /*!
     * \brief Gets specified parameter.
     * \param cmdType - Type of command that should be returned.
     * \param id - Id of command that should be returned.
     * \param cmdParam - CustomCmdParameter that should be returned.
     * \return Value of specified parameter. Will be empty if something goes wrong.
     */
    static QString GetParameter(CmdType cmdType, int id, CustomCmdParameter cmdParam);
    /*!
     * \brief Sets new value to specified parameter.
     * \param cmdType - Type of command that should be updated.
     * \param cmdName - Name of command that should be updated.
     * \param cmdParam - CustomCmdParameter that should be updated.
     * \param value - New value for specified parameter.
     */
    void SetParameter(CmdType cmdType, const QString &cmdName, CustomCmdParameter cmdParam, const QString &value);
    /*!
     * \brief Updates covenant field for all commands that have specified value.
     * \param cmdType - Type of command that should be updated.
     * \param currentCovenantName - covenant name that was changed.
     * \param newCovenantName - new covenant name.
     */
    static void UpdateCovenantName(CmdType cmdType, const QString &currentCovenantName, const QString &newCovenantName);

    /*** Command answers ***/
    /*!
     * \brief Gets array of answers for specified command.
     * \param cmdType - Type of command that should be returned.
     * \param cmdName - Name of command that should be returned.
     * \return Array of ids of answers for specified command.
     */
    static QVector<int> GetAnswers(CmdType cmdType, const QString &cmdName);
    /*!
     * \brief Gets random answer for specified command.
     * \param cmdType - Type of command that should be returned.
     * \param cmdName - Name of command that should be returned.
     * \return Random answer from specified command.
     */
    static QString GetRandomAnswer(CmdType cmdType, const QString &cmdName);
    /*!
     * \brief Gets specified answer.
     * \param cmdType - Type of command that should be returned.
     * \param id - Id of command that should be returned.
     * \return Specified answer.
     */
    static QString GetAnswer(CmdType cmdType, int id);
    /*!
     * \brief Adds new answer to specified command.
     * \param cmdType - Type of command that should be updated.
     * \param cmdName - Name of command that should be updated.
     * \param answer - Answer that should be added to specified command.
     */
    void AddAnswer(CmdType cmdType, const QString &cmdName, const QString &answer);
    /*!
     * \brief Edits specified command answer.
     * \param cmdType - Type of command that should be updated. 
     * \param id - Id of command answer that should be updated.
     * \param answer - New value for specified command answer.
     */
    void EditAnswer(CmdType cmdType, int id, const QString &answer);
    /*!
     * \brief Delete specified answer.
     * \param cmdType - Type of command that should be updated.
     * \param id - Id of command answer that should be deleted.
     */
    void DeleteAnswer(CmdType cmdType, int id);

signals:
    /*** Commands ***/
    /*!
     * \brief Signal which notify about new custom command.
     * \param cmdType - Type of command that was added.
     * \param cmdName - Name of command that was added.
     * \param id - Id of command that was added.
     */
    void CustomCmdAdded(CmdType cmdType, const QString &cmdName, int id);
    /*!
     * \brief Signal which notify about deletion custom command.
     * \param cmdType - Type of command that was deleted.
     * \param cmdName - Name of command that was deleted.
     * \param id - Id of command that was deleted.
     */
    void CustomCmdDeleted(CmdType cmdType, const QString &cmdName, int id);

    /*** Commands created by user ***/
    /*!
     * \brief Signal to notify about change of custom command parameter.
     * \param cmdName - Name of command that was changed.
     * \param cmdParam - CustomCmdParameter that was changed.
     * \param value - New value of parameter.
     */
    void CustomCmdParameterChanged(const QString &cmdName, CustomCmdParameter cmdParam, const QString &value);
    /*!
     * \brief Signal to notify about new answer that was added to custom command.
     * \param cmdName - Command to which new answer was added.
     */
    void CustomCmdAnswerAdded(const QString &cmdName);
    /*!
     * \brief Signal to notify about change of custom command answer.
     * \param cmdName - Name of command that was updated.
     * \param id - Id of custom command answer that was edited.
     */
    void CustomCmdAnswerEdited(const QString &cmdName, int id);
    /*!
     * \brief Signal to notify about deletion of custom command answer.
     * \param cmdName - Name of command that was updated.
     * \param id - Id of custom command answer that was deleted.
     */
    void CustomCmdAnswerDeleted(const QString &cmdName, int id);

    /*** Commands created by covenants ***/
    /*!
     * \brief Signal to notify about change of custom covenant command parameter.
     * \param cmdName - Name of command that was changed.
     * \param cmdParam - CustomCmdParameter that was changed.
     * \param value - New value of parameter.
     */
    void CustomCovCmdParameterChanged(const QString &cmdName, CustomCmdParameter cmdParam, const QString &value);
    /*!
     * \brief Signal to notify about new answer that was added to custom covenant command.
     * \param cmdName - Command to which new answer was added.
     */
    void CustomCovCmdAnswerAdded(const QString &cmdName);
    /*!
     * \brief Signal to notify about change of custom covenant command answer.
     * \param cmdName - Name of command that was updated.
     * \param id - Id of custom covenant command answer that was edited.
     */
    void CustomCovCmdAnswerEdited(const QString &cmdName, int id);
    /*!
     * \brief Signal to notify about deletion of custom covenant command answer.
     * \param cmdName - Name of command that was updated.
     * \param id - Id of custom covenant command answer that was deleted.
     */
    void CustomCovCmdAnswerDeleted(const QString &cmdName, int id);

private:
    /*!
     * \brief Simple enumeration to identify which table should be used.
     */
    enum class TableType
    {
        Commands,
        Answers
    };

    /*!
     * \brief Gets table name that depends on table type and command type.
     * \param tableType - Type of table.
     * \param cmdType - Type of command.
     * \return Table name that was generated.
     * 
     * Generates table name that will be used to do something with custom commands.
     */
    static QString _GetTableName(TableType tableType, CmdType cmdType);
    /*!
     * \brief Initializes custom command table (list of commands and list of answers).
     * \param mainTableName - Main part of table name.
     * \param indexPrefix - Prefix for index names.
     * 
     * Created needed database tables and prefixes in there.
     */
    static QString _InititalizeCustomCommandTables(const QString &mainTableName, const QString &indexPrefix);
};

}
}
