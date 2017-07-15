/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MessageGraphWidget.hpp"
#include "Utils/UserData/RealTimeUserData.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Ui::Analytics;
using namespace QtCharts;

///////////////////////////////////////////////////////////////////////////

MessageGraphWidget::MessageGraphWidget(QWidget* parent) : QGroupBox(parent)
{
    _startingTime = QDateTime::currentDateTime();
    this->setTitle("Message graph");
    // Inititalize chart
    _series = new QLineSeries();
    _chart = new QChart();
    _chart->addSeries(_series);
    _chart->legend()->hide();
    _chart->setTitle("Average: ");
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
    // Initialize timer
    _timer = new QTimer(this);
    UpdateGraph();
    connect(_timer, &QTimer::timeout,
            this, &MessageGraphWidget::UpdateGraph);
    // Start timer
    ConfigurationManager::Instance().GetStringParam(CFGP_MESSAGE_GRAPH_UPD_TIME, _timeToUpdate);
    _timer->start(_timeToUpdate.toInt());
}

///////////////////////////////////////////////////////////////////////////

MessageGraphWidget::~MessageGraphWidget() {}

///////////////////////////////////////////////////////////////////////////

void MessageGraphWidget::UpdateGraph()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    _series->append(currentTime.toMSecsSinceEpoch(), RealTimeUserData::Instance()->GetMsgCounter());
    _dateAxis->setRange(_startingTime, currentTime);
    int newMaxY = RealTimeUserData::Instance()->GetMsgCounter() + 1.0;
    _valueAxis->setMax(newMaxY);
    if (newMaxY <= 6)
    {
        _valueAxis->setTickCount(newMaxY+1);
    }
    else
    {
        _valueAxis->setTickCount(7);
    }
    ConfigurationManager::Instance().GetStringParam(CFGP_MESSAGE_GRAPH_UPD_TIME, _timeToUpdate);
    _timer->start(_timeToUpdate.toInt());
    // Update average msg counter
    _UpdateAverageMsg();
}

///////////////////////////////////////////////////////////////////////////

void MessageGraphWidget::_UpdateAverageMsg()
{
    QVector<QPointF> values = _series->pointsVector();
    if (values.size() > 1)
    {
        unsigned long long sum(0);
        for (int i = 1; i < values.size(); ++i)
        {
            sum += (unsigned long long)values.at(i).toPoint().y() - (unsigned long long)values.at(i - 1).toPoint().y();
        }
        if (sum == 0)
        {
            _chart->setTitle("Average: 0");
        }
        else
        {
            _chart->setTitle(QString("Average: %1").arg(sum/values.size()));
        }
    }
}

///////////////////////////////////////////////////////////////////////////
