/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ViewerCounterWidget.hpp"

using namespace Ui::Analytics;

///////////////////////////////////////////////////////////////////////////

ViewerCounterWidget::ViewerCounterWidget(QWidget* parent) : QGroupBox(parent)
{
    this->setTitle("Viewers");
    _layout = new QVBoxLayout();
    _layout->setAlignment(Qt::AlignTop);
    _layout->setMargin(10);
    this->setLayout(_layout);
    _AddViewerLabels();
    RealTimeUserData* realTimeUD = RealTimeUserData::Instance();
    connect(realTimeUD, &RealTimeUserData::UserListChanged,
            this, &ViewerCounterWidget::UpdateLabels);
}

///////////////////////////////////////////////////////////////////////////

ViewerCounterWidget::~ViewerCounterWidget() {}

///////////////////////////////////////////////////////////////////////////

void ViewerCounterWidget::UpdateLabels()
{
    int maxUserNumber = RealTimeUserData::Instance()->GetMaxUserNumber();
    int curUserNumber = RealTimeUserData::Instance()->GetUserList().size();
    _maxViewers->setText("Maximum: " + QString::number(maxUserNumber));
    _curViewers->setText("Current: " + QString::number(curUserNumber));
}

///////////////////////////////////////////////////////////////////////////

void ViewerCounterWidget::_AddViewerLabels()
{
    // Add and initialize curretn viewer label
    _curViewers = new QLabel(this);
    _maxViewers = new QLabel(this);
    _layout->addWidget(_curViewers);
    _layout->addWidget(_maxViewers);
    _curViewers->setText("Current: 0");
    _maxViewers->setText("Maximum: 0");
}

///////////////////////////////////////////////////////////////////////////
