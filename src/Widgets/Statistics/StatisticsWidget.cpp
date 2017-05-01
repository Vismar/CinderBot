#include "StatisticsWidget.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

StatisticsWidget::StatisticsWidget(QWidget* parent) : QScrollArea(parent)
{
    setWidgetResizable(true);
    _container = new QWidget();
    this->setWidget(_container);
    _layout = new QVBoxLayout(_container);
    _layout->setAlignment(Qt::AlignTop);
    _layout->setMargin(10);

    // Add widgets
    _AddViewerStatWidget();
    _AddMessageStatWidget();
}

///////////////////////////////////////////////////////////////////////////

StatisticsWidget::~StatisticsWidget() {}

///////////////////////////////////////////////////////////////////////////

void StatisticsWidget::_AddViewerStatWidget()
{
    _viewerStatWidget = new ViewerStatisticsWidget(this);
    _layout->addWidget(_viewerStatWidget);
}

///////////////////////////////////////////////////////////////////////////

void StatisticsWidget::_AddMessageStatWidget()
{
    _msgStatWidget = new MessageStatisticsWidget(this);
    _layout->addWidget(_msgStatWidget);
}

///////////////////////////////////////////////////////////////////////////
