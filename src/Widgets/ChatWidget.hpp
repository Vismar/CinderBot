#pragma once
#include "../Chat/ChatMessage.hpp"
#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>

namespace Ui
{

class ChatWidget : public QScrollArea
{
    Q_OBJECT
private:
    QVBoxLayout* _layout;
    QWidget*     _container;

public:
    explicit ChatWidget(QWidget* parent = 0);
    ~ChatWidget();

public slots:
    void AddEntry(ChatMessage message, bool botMessage);

private slots:
    void _AutoScrollDown();
};

}
