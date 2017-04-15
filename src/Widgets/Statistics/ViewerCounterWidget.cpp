#include "ViewerCounterWidget.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

ViewerCounterWidget::ViewerCounterWidget(QWidget* parent) : QGroupBox(parent)
{
    this->setTitle("Viewers");
    _layout = new QVBoxLayout();
    _layout->setAlignment(Qt::AlignTop);
    _layout->setMargin(10);
    this->setLayout(_layout);
    _curViewerNumber = 0;
    _maxViewerNumber = 0;
    _AddViewerLabels();
    RealTimeUserData* realTimeUD = RealTimeUserData::Instance();
    connect(realTimeUD, &RealTimeUserData::UserListChanged,
            this, &ViewerCounterWidget::UpdateLabels);
}

///////////////////////////////////////////////////////////////////////////

ViewerCounterWidget::~ViewerCounterWidget() {}

///////////////////////////////////////////////////////////////////////////

void ViewerCounterWidget::UpdateLabels()
{
    const QStringList& userList = RealTimeUserData::Instance()->GetUserList();
    _curViewerNumber = userList.size();
    if (_curViewerNumber > _maxViewerNumber)
    {
        _maxViewerNumber = _curViewerNumber;
        _maxViewers->setText("Maximum: " + QString::number(_maxViewerNumber));
    }
    _curViewers->setText("Current: " + QString::number(_curViewerNumber));
}

///////////////////////////////////////////////////////////////////////////

void ViewerCounterWidget::_AddViewerLabels()
{
    // Add and initialize curretn viewer label
    _curViewers = new QLabel(this);
    _maxViewers = new QLabel(this);
    _layout->addWidget(_curViewers);
    _layout->addWidget(_maxViewers);
    _curViewers->setText("Current: " + QString::number(_curViewerNumber));
    _maxViewers->setText("Maximum: " + QString::number(_maxViewerNumber));
}

///////////////////////////////////////////////////////////////////////////
