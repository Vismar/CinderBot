/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QGroupBox>
#include <QHBoxLayout>
#include "MessageGraphWidget.hpp"

namespace Ui
{

class MessageStatisticsWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit MessageStatisticsWidget(QWidget* parent = 0);
    ~MessageStatisticsWidget();

private:
    //void _AddMsgLimitsWidget();
    /*!
     * Add and initialize MsgGraph widget
     */
    void _AddMsgGraphWidget();

    /*!  */
    QHBoxLayout* _layout;
    /*!  */
    //MessageLimitsWidget* _msgLimits;
    /*! Message graph widget */
    MessageGraphWidget* _msgGraph;
};

}
