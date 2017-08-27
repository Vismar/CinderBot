/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "ViewerCounterWidget.hpp"
#include "ViewerGraphWidget.hpp"

namespace Ui
{
namespace Analytics
{

class ViewerStatisticsWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewerStatisticsWidget(QWidget* parent = 0);
    ~ViewerStatisticsWidget();

private:
    /*!
     * Add and initialize ViewerCounter widget
     */
    void _AddViewerCounterWidget();
    /*!
     * Add and initialize ViewerGraph widget
     */
    void _AddViewerGraphWidget();

    /*! Horizontal layout */
    QHBoxLayout* _layout;
    /*! ViewerCounter widget*/
    ViewerCounterWidget* _viewerCounter;
    ViewerGraphWidget* _viewerGraph;
};

}
}
