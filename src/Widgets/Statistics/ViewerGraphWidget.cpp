#include "ViewerGraphWidget.hpp"
#include <QTime>
#include <Utils/UserData/RealTimeUserData.hpp>
#include <Utils/Config/ConfigurationManager.hpp>
#include <Utils/Config/ConfigurationParameters.hpp>

using namespace Ui;
using namespace QtCharts;

///////////////////////////////////////////////////////////////////////////

ViewerGraphWidget::ViewerGraphWidget(QWidget* parent) : QGroupBox(parent)
{
    _startingTime = QDateTime::currentDateTime();
    this->setTitle("Viewer graph");
    // Inititalize chart
    _series = new QLineSeries();
    _chart = new QChart();
    _chart->addSeries(_series);
    _chart->legend()->hide();
    // Initialize date axis
    _dateAxis = new QDateTimeAxis();
    _dateAxis->setTickCount(5);
    _dateAxis->setFormat("MMM dd<br>h:mm:ss");
    _chart->addAxis(_dateAxis, Qt::AlignBottom);
    _series->attachAxis(_dateAxis);
    // Initialize value axis
    _valueAxis = new QValueAxis();
    _valueAxis->setTickCount(2);
    _valueAxis->setLabelFormat("%.1f");
    _chart->addAxis(_valueAxis, Qt::AlignLeft);
    _series->attachAxis(_valueAxis);
    // Initialize view
    _chartView = new QChartView(_chart);
    _chartView->setFixedHeight(200);
    _chartView->setRenderHint(QPainter::Antialiasing);
    // Initialize layout
    _layout = new QHBoxLayout();
    _layout->setMargin(10);
    _layout->setAlignment(Qt::AlignTop);
    this->setLayout(_layout);
    _layout->addWidget(_chartView);
    // Initizlie timer
    _timer = new QTimer(this);
    UpdateGraph();
    connect(_timer, &QTimer::timeout,
            this, &ViewerGraphWidget::UpdateGraph);
    // Start timer
    ConfigurationManager::Instance().GetStringParam(CFGP_VGRAPH__UPD_TIME, _timeToUpdate);
    _timer->start(_timeToUpdate.toInt());
}

///////////////////////////////////////////////////////////////////////////

ViewerGraphWidget::~ViewerGraphWidget() {}

///////////////////////////////////////////////////////////////////////////

void ViewerGraphWidget::UpdateGraph()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    _series->append(currentTime.toMSecsSinceEpoch(), RealTimeUserData::Instance()->GetUserList().size());
    _dateAxis->setRange(_startingTime, currentTime);
    int newMaxY = RealTimeUserData::Instance()->GetMaxUserNumber() + 1.0;
    _valueAxis->setMax(newMaxY);
    if (newMaxY <= 6)
    {
        _valueAxis->setTickCount(newMaxY+1);
    }
    else
    {
        _valueAxis->setTickCount(7);
    }
    ConfigurationManager::Instance().GetStringParam(CFGP_VGRAPH__UPD_TIME, _timeToUpdate);
    _timer->start(_timeToUpdate.toInt());
}

///////////////////////////////////////////////////////////////////////////
