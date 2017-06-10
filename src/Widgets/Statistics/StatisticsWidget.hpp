/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "ViewerStatisticsWidget.hpp"
#include "MessageStatisticsWidget.hpp"

namespace Ui
{

class StatisticsWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit StatisticsWidget(QWidget* parent = 0);
    ~StatisticsWidget();

private:
    /*!
     * Create and initialize viewer statistics widget
     */
    void _AddViewerStatWidget();
    /*!
     * Create and initialize message statistics widget
     */
    void _AddMessageStatWidget();

    /*! Vertical layout */
    QVBoxLayout* _layout;
    /*! Simple container to store scrol layout */
    QWidget*     _container;
    /*! ViewerStatistics widget */
    ViewerStatisticsWidget* _viewerStatWidget;
    /*! MessageStatistics widget */
    MessageStatisticsWidget* _msgStatWidget;
};

}
