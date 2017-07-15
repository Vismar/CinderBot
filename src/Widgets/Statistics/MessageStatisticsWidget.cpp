/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MessageStatisticsWidget.hpp"

using namespace Ui::Analytics;

///////////////////////////////////////////////////////////////////////////

MessageStatisticsWidget::MessageStatisticsWidget(QWidget* parent) : QGroupBox(parent)
{
    this->setTitle("Message statistics");
    _layout = new QHBoxLayout();
    _layout->setAlignment(Qt::AlignLeft);
    _layout->setMargin(10);
    this->setLayout(_layout);
    this->setFixedHeight(275);;

    // Add widgets
    _AddMsgGraphWidget();
}

///////////////////////////////////////////////////////////////////////////

MessageStatisticsWidget::~MessageStatisticsWidget() {}

///////////////////////////////////////////////////////////////////////////

void MessageStatisticsWidget::_AddMsgGraphWidget()
{
    _msgGraph = new MessageGraphWidget(this);
    _layout->addWidget(_msgGraph);
}

///////////////////////////////////////////////////////////////////////////
