/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QDialog>
#include <QPushButton>
#include "ParametersCustomCommandWidget.hpp"

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
 * \brief Window to create custom command.
 *
 * Simple widget which contains all needed params to create custom command. 
 * After creating a command opens the EditCustomCommandWindow.
 */
class CreateCustomCommandWindow : public QDialog
{
    Q_OBJECT
public:
    explicit CreateCustomCommandWindow(QWidget *parent = 0);
    ~CreateCustomCommandWindow();

signals:
    /*!
     * \brief Signaling about new command that was created.
     * \param cmdName - name of new command.
     *
     * Notifies that new command was created and sends the name of created command.
     */
    void NewCommand(const QString &cmdName);

private slots:
    /*!
     * \brief Checks if command name field have text.
     * \param text - new text in command name field.
     *
     * Check if command field have a text to handle.
     */
    void _CheckCommandNameField(const QString &text) const;
    /*!
     * \brief Handles "Create" button.
     *
     * Sets params and tries to create custom command in database.
     */
    void _OnCreateButton();

private:
    /*! Main layout of window */
    QVBoxLayout *_layout;
    /*! Parameters of a new custom command */
    ParametersCustomCommandWidget *_parameters;
    /*! Create button */
    QPushButton *_createButton;
};

}
}
