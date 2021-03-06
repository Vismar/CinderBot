/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "StatisticsWidget.hpp"

using namespace Ui::Analytics;

///////////////////////////////////////////////////////////////////////////

StatisticsWidget::StatisticsWidget(QWidget* parent) : QScrollArea(parent)
{
    setWidgetResizable(true);
    _container = new QWidget();
    this->setWidget(_container);
    _layout = new QVBoxLayout(_container);
    _layout->setAlignment(Qt::AlignTop);
    _layout->setMargin(10);

    // Add widgets
    _AddViewerStatWidget();
    _AddMessageStatWidget();
}

///////////////////////////////////////////////////////////////////////////

StatisticsWidget::~StatisticsWidget() {}

///////////////////////////////////////////////////////////////////////////

void StatisticsWidget::_AddViewerStatWidget()
{
    _viewerStatWidget = new ViewerStatisticsWidget(this);
    _layout->addWidget(_viewerStatWidget);
}

///////////////////////////////////////////////////////////////////////////

void StatisticsWidget::_AddMessageStatWidget()
{
    _msgStatWidget = new MessageStatisticsWidget(this);
    _layout->addWidget(_msgStatWidget);
}

///////////////////////////////////////////////////////////////////////////
