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

// TODO: Add comments for class
class ListCustomCommandWidget : public Common::PageListWidget
{
public:
    explicit ListCustomCommandWidget(QWidget *parent = 0);
    virtual ~ListCustomCommandWidget();

protected:
    ////////////////////////////////
    /// PageListWidget override
    void _CreateAndAddWidget();
    void _UpdateEntry(QWidget *entry, int id);
};

}
}
