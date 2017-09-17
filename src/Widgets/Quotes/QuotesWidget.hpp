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
 * \brief All quote related widgets.
 */
namespace Quote
{

/*!
 * \brief Class to store and display quotes as paged list.
 */
class QuotesWidget : public Common::PageListWidget
{
public:
    explicit QuotesWidget(QWidget *parent = nullptr);
    virtual ~QuotesWidget();

protected:
    /*///////////////////////////////*/
    /*   PageListWidget overrides   */
    /*///////////////////////////////*/
    /*!
     * \brief Ñreates specific widget.
     */
    void _CreateAndAddWidget() override;
    /*!
     * \brief Update specified widget with data related to specified id.
     * \param entry - pointer to widget which should be updated.
     * \param id - data id.
     */
    void _UpdateEntry(QWidget *entry, int id) override;
};

}
}
