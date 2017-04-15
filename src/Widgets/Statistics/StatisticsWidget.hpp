#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "ViewerStatisticsWidget.hpp"

namespace Ui
{

class StatisticsWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit StatisticsWidget(QWidget* parent = 0);
    ~StatisticsWidget();

private:
    /*!
     * Create and initialize viewer statistics widget
     */
    void _AddViewerStatWidget();

    /*! Vertical layout */
    QVBoxLayout* _layout;
    /*! Simple container to store scrol layout */
    QWidget*     _container;
    /*! ViewerStatistics widget */
    ViewerStatisticsWidget* _viewerStatWidget;
};

}
