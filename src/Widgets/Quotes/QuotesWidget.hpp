/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Widgets/Common/PageListWidget.hpp"

namespace Ui
{

class QuotesWidget : public PageListWidget
{
public:
    /*!
     * Constructor
     */
    explicit QuotesWidget(QWidget *parent = 0);
    /*!
     * Destructor
     */
    virtual ~QuotesWidget();

protected:
    ////////////////////////////////
    /// PageListWidget override
    void _CreateAndAddWidget();
    void _UpdateEntry(QWidget *entry, int id);
};

}
