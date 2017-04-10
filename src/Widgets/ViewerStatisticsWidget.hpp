#pragma once
#include <QGroupBox>
#include <QHBoxLayout>

namespace Ui
{

class ViewerStatisticsWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewerStatisticsWidget(QWidget* parent = 0);
    ~ViewerStatisticsWidget();

private:
    /*! Horizontal layout */
    QHBoxLayout* _layout;
};

}
