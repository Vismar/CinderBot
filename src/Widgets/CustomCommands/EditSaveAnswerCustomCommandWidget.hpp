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

/*!
 * \brief Widget that contains answer text and capable of saving/editing/deleting it.
 *
 * Handles "Save" button to update answer data, also handles "Delete" button to delete answer.
 */
class EditSaveAnswerCustomCommandWidget : public Common::EditSaveWidget
{
public:
    explicit EditSaveAnswerCustomCommandWidget(QWidget* parent = 0);
    virtual ~EditSaveAnswerCustomCommandWidget();

protected:
    /*////////////////////////////*/
    /* EditSaveWidget override    */
    /*////////////////////////////*/
    /*!
     * \brief Saves new answer for current command.
     * 
     * Gets text from _text field and saves it as new answer for current command.
     */
    void _SaveEntry() override;
    /*!
     * \brief Deleted current answer from current command.
     * 
     * Get _id and uses it to delete answer from current command.
     */
    void _DeleteEntry() override;
};

}
}
