#pragma once
#include <QGroupBox>
#include <QHBoxLayout>
#include "ViewerCounterWidget.hpp"

namespace Ui
{

class ViewerStatisticsWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewerStatisticsWidget(QWidget* parent = 0);
    ~ViewerStatisticsWidget();

private:
    /*!
     * Add and initialize ViewerCounter widget
     */
    void _AddViewerCounterWidget();

    /*! Horizontal layout */
    QHBoxLayout* _layout;
    /*! ViewerCounter widget*/
    ViewerCounterWidget* _viewerCounter;
};

}
