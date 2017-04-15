#include "ViewerStatisticsWidget.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

ViewerStatisticsWidget::ViewerStatisticsWidget(QWidget* parent) : QGroupBox(parent)
{
    this->setTitle("Viewer statistics");
    _layout = new QHBoxLayout();
    _layout->setAlignment(Qt::AlignLeft);
    _layout->setMargin(10);
    this->setLayout(_layout);

    // Add widgets
    _AddViewerCounterWidget();
}

///////////////////////////////////////////////////////////////////////////

ViewerStatisticsWidget::~ViewerStatisticsWidget() {}

///////////////////////////////////////////////////////////////////////////

void ViewerStatisticsWidget::_AddViewerCounterWidget()
{
    _viewerCounter = new ViewerCounterWidget(this);
    _layout->addWidget(_viewerCounter);
}

///////////////////////////////////////////////////////////////////////////
