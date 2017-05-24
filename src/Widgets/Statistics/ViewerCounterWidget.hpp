/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <Utils/UserData/RealTimeUserData.hpp>

namespace Ui
{

class ViewerCounterWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewerCounterWidget(QWidget* parent = 0);
    ~ViewerCounterWidget();

public slots:
    void UpdateLabels();

private:
    /*!
     * Add labels which represents current, max and average number of viewers
     */
    void _AddViewerLabels();

    /*! Vertical layout */
    QVBoxLayout* _layout;
    /*! Current viewer counter */
    QLabel* _curViewers;
    /*! Max viewer number */
    QLabel* _maxViewers;
};

}
