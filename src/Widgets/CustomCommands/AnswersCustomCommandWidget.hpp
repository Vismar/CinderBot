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
 * \brief Class to store and display answers fro custom command.
 *
 * Inherited PageList Widget to display all answers for specific custom command.
 */
class AnswersCustomCommandWidget : public Common::PageListWidget
{
public:
    explicit AnswersCustomCommandWidget(QWidget *parent = 0);
    virtual ~AnswersCustomCommandWidget();

protected:
    ////////////////////////////////
    /// PageListWidget override
    void _CreateAndAddWidget();
    void _UpdateEntry(QWidget *entry, int id);
};

}
}
