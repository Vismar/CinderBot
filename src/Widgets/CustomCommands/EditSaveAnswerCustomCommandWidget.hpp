/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Widgets/Common/EditSaveWidget.hpp"

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

class EditSaveAnswerCustomCommandWidget : public Common::EditSaveWidget
{
public:
    explicit EditSaveAnswerCustomCommandWidget(QWidget* parent = 0);
    virtual ~EditSaveAnswerCustomCommandWidget();

protected:
    ////////////////////////////////
    /// EditSaveWidget override
    void _SaveEntry();
    void _DeleteEntry();
};

}
}
