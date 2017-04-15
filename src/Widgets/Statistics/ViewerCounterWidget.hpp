#pragma once
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>

namespace Ui
{

class ViewerCounterWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewerCounterWidget(QWidget* parent = 0);
    ~ViewerCounterWidget();

public slots:
    void UpdateLabels(int newCurViewers);

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
    /*! Current viewers number */
    int _curViewerNumber;
    /*! Max viewers number */
    int _maxViewerNumber;
};

}
