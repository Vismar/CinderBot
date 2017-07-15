/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ViewerStatisticsWidget.hpp"

using namespace Ui::Analytics;

///////////////////////////////////////////////////////////////////////////

ViewerStatisticsWidget::ViewerStatisticsWidget(QWidget* parent) : QGroupBox(parent)
{
    this->setTitle("Viewer statistics");
    _layout = new QHBoxLayout();
    _layout->setAlignment(Qt::AlignLeft);
    _layout->setMargin(10);
    this->setLayout(_layout);
    this->setFixedHeight(275);

    // Add widgets
    _AddViewerCounterWidget();
    _AddViewerGraphWidget();
}

///////////////////////////////////////////////////////////////////////////

ViewerStatisticsWidget::~ViewerStatisticsWidget() {}

///////////////////////////////////////////////////////////////////////////

void ViewerStatisticsWidget::_AddViewerCounterWidget()
{
    _viewerCounter = new ViewerCounterWidget(this);
    _layout->addWidget(_viewerCounter);
}

///////////////////////////////////////////////////////////////////////////

void ViewerStatisticsWidget::_AddViewerGraphWidget()
{
    _viewerGraph = new ViewerGraphWidget(this);
    _layout->addWidget(_viewerGraph);
}

///////////////////////////////////////////////////////////////////////////
