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
 * \brief All widgets related to custom commands.
 */
namespace CustomCommand
{

/*!
 * \brief Class to store and display answers for custom command.
 *
 * Inherited PageListWidget to display all answers for specific custom command.
 */
class AnswersCustomCommandWidget : public Common::PageListWidget
{
public:
    explicit AnswersCustomCommandWidget(QWidget *parent = nullptr);
    virtual ~AnswersCustomCommandWidget();

protected:
    /*////////////////////////////*/
    /* PageListWidget override    */
    /*////////////////////////////*/
    /*!
     * \brief Creates entry widget.
     * 
     * Creates EditSaveAnswerCustomCommandWidget as entry widget and call _AddWidget() to initialize it.
     */
    void _CreateAndAddWidget() override;
    /*!
     * \brief Updates answer in specified entry by id.
     * \param entry - pointer to entry widget which should be updated.
     * \param id - id of answer which should be used to update entry.
     * 
     * Gets answer by id from CustomCommandDBHelper and sets it to specified entry widget.
     */
    void _UpdateEntry(QWidget *entry, int id) override;
};

}
}
