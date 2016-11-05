#pragma once
#include <QListWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "../Chat/ChatMessage.hpp"

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
