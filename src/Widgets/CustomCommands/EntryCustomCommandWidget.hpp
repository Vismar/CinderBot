/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

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
 * \brief Widget to display command name and to edit/delete it.
 * 
 * Simple widget that contains "Command name" text field and two buttons: "Edit" and "Delete".
 * "Edit" button opens EditCustomCommandWindow. "Delete" button deletes current command.
 */
class EntryCustomCommandWidget : public QFrame
{
    Q_OBJECT
public:
    explicit EntryCustomCommandWidget(QWidget* parent = 0);
    virtual ~EntryCustomCommandWidget();

    /*!
     * \brief Sets command name to text field.
     * \param cmdName - text which will be used in text field to represent command name.
     * 
     * Sets specified command name in text field to display it.
     */
    void SetCmdName(const QString &cmdName) const;

private slots:
    /*!
     * \brief Handle "Edit" button.
     * 
     * Shows EditCustomCommandWindow in which user can edit selected command.
     */
    void _HandleEditButton() const;
    /*!
     * \brief Handle "Delete" button.
     * 
     * Deletes selected command.
     */
    void _HandleDeleteButton() const;

private:
    /*! Widget layout */
    QHBoxLayout* _layout;
    /*! Command name field */
    QLineEdit*   _cmdName;
    /*! Edit button */
    QPushButton* _editButton;
    /*! Delete button */
    QPushButton* _deleteButton;
};

}
}
