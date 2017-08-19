/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Widgets/Common/PageListWidget.hpp"

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
 * \brief Widget that contains and displays all commands.
 * 
 * Inherited PageListWidget to display all custom commands created by user.
 */
class ListCustomCommandWidget : public Common::PageListWidget
{
public:
    explicit ListCustomCommandWidget(QWidget *parent = 0);
    virtual ~ListCustomCommandWidget();

protected:
    /*////////////////////////////*/
    /* PageListWidget override    */
    /*////////////////////////////*/
    /*!
     * \brief Creates entry widget.
     * 
     * Creates EntryCustomCommandWidget as entry widget and call _AddWidget() to initialize it.
     */
    void _CreateAndAddWidget() override;
    /*!
     * \brief Updates command in specified entry by id.
     * \param entry - pointer to entry widget which should be updated.
     * \param id - id of command which should be used to update entry.
     * 
     * Gets command by id from CustomCommandDBHelper and sets it to specified entry widget.
     */
    void _UpdateEntry(QWidget *entry, int id) override;
};

}
}
