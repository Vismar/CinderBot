/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "StatisticsWidget.hpp"

namespace Ui
{
namespace Analytics
{

class StatisticsWindow : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit StatisticsWindow(QWidget* parent = 0);
    /*!
     * Destructor
     */
    ~StatisticsWindow();

private:
    /*! Widget layout */
    QVBoxLayout*       _layout;
    /*! Statistics widget */
    StatisticsWidget* _statisticsWidget;
};

}
}
