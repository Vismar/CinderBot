/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "StatisticsWindow.hpp"

using namespace Ui::Analytics;

///////////////////////////////////////////////////////////////////////////

StatisticsWindow::StatisticsWindow(QWidget *parent) : QWidget(parent, Qt::Window)
{
    // Initialize dialog window
    this->setWindowTitle("Statistics");
    this->setMinimumSize(450, 330);
    // Initialize layout
    _layout = new QVBoxLayout(this);
    _layout->setMargin(0);
    _layout->setAlignment(Qt::AlignTop);

    // Initialize create quote widget
    _statisticsWidget = new StatisticsWidget(this);
    _layout->addWidget(_statisticsWidget);
}

///////////////////////////////////////////////////////////////////////////

StatisticsWindow::~StatisticsWindow() { }

///////////////////////////////////////////////////////////////////////////
