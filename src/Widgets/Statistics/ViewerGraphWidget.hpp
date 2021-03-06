/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QGroupBox>
#include <QtCharts/QChartView>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QLineSeries>
#include <QHBoxLayout>
#include <QTimer>
#include <QDateTime>

namespace Ui
{
namespace Analytics
{

class ViewerGraphWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewerGraphWidget(QWidget* parent = 0);
    ~ViewerGraphWidget();

public slots:
    void UpdateGraph();

private:
    QtCharts::QChartView* _chartView;
    QtCharts::QChart* _chart;
    QtCharts::QDateTimeAxis* _dateAxis;
    QtCharts::QValueAxis* _valueAxis;
    QtCharts::QLineSeries* _series;
    QHBoxLayout* _layout;
    QTimer* _timer;
    QDateTime _startingTime;
    QString _timeToUpdate;
};

}
}
