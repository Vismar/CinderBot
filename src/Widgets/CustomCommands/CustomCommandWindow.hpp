/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Widgets/CustomCommands/ListCustomCommandWidget.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

/*!
* \brief All Ui widgets and windows.
*/
namespace Ui
{
/*!
 * \brief All widgets related to custom commands.
 */
namespace CustomCommand
{

/*!
 * \brief This window displays all custom commands.
 *
 * Contains simple list of all custom commands and handle "Create" button to open CreateCustomCommandWindow,
 * handle "Edit" button to open EditCustomCommandWindow, handle "Delete" button to delete custom command.
 */
class CustomCommandWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CustomCommandWindow(QWidget *parent = 0);
    ~CustomCommandWindow();

private slots:
    /*!
     * \brief Open dialog box for creating new command.
     *
     * Opens dialog box to create new custom command and then saves it.
     */
    void _CreateCommand() const;
    /*!
     * \brief Adds created command to list.
     *
     * Adds created command id to list which will add needed entry.
     */
    void _AddCommand(Utils::Database::CmdType cmdType, const QString &cmdName, int commandId) const;
    /*!
     * \brief Deletes specific command from list.
     *
     * Deletes command id from list which will remove needed entry.
     */
    void _DeleteCommand(Utils::Database::CmdType cmdType, const QString &cmdName, int commandId) const;
    /*!
     * \brief Opens new window to edit new command.
     * \param cmdName - command name which should be edited.
     *
     * Opens window to edit specific custom command.
     */
    void _EditNewCommand(const QString &cmdName) const;

private:
    /*!
     * \brief Loads all commands to ListCustomCommandWidget.
     *
     * Get all command ids from CistCommandDBHelper and sends it to command list. 
     */
    void _LoadCommands() const;

    /*! Main layout of window */
    QVBoxLayout *_mainLayout;
    /*! Create button */
    QPushButton *_createButton;
    /*! List of commands */
    ListCustomCommandWidget *_commandList;
};

}
}
