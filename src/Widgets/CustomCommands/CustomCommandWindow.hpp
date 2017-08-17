/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QVBoxLayout>
#include <QPushButton>
#include "Widgets/CustomCommands/ListCustomCommandWidget.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

/*!
* \brief All Ui widgets and windows.
*/
namespace Ui
{
/*!
 * \brief All wigets related to custom commands.
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
     * Opens dialog box to create new custommand and then save it.
     */
    void _CreateCommand();
    /*!
     * \brief
     *
     * 
     */
    void _AddCommand(Utils::Database::CmdType cmdType, const QString &cmdName, int commandId);
    /*!
     * \brief
     *
     *
     */
    void _DeleteCommand(Utils::Database::CmdType cmdType, const QString &cmdName, int commandId);
    /*!
     * \brief Open new window to edit new command.
     * \param cmdName - command name which should be edited.
     *
     * Opens window to edit specific custom command.
     */
    void _EditNewCommand(const QString &cmdName);

private:
    /*!
     * \brief Loads all command to ListCustomCommandWidget.
     *
     * Get all command ids and 
     */
    void _LoadCommands();

    /*! Main layout of window */
    QVBoxLayout *_mainLayout;
    /*! Create button */
    QPushButton *_createButton;
    /*! List of commands */
    ListCustomCommandWidget *_commandList;
};

}
}
