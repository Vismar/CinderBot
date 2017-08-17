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
 * \brief All wigets related to custom commands.
 */
namespace CustomCommand
{

class EntryCustomCommandWidget : public QFrame
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit EntryCustomCommandWidget(QWidget* parent = 0);
    /*!
     * Destructor
     */
    virtual ~EntryCustomCommandWidget();

    /*!
     * Set text field for quote
     * \param text - text which will be used in text field
     */
    void SetCmdName(const QString &cmdName);

private slots:
    /*!
     * Handle edit button
     */
    void _HandleEditButton();
    /*!
     * Handle delete button
     */
    void _HandleDeleteButton();

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
