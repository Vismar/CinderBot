#include "ChatWidget.hpp"
#include <QListWidgetItem>
#include <QScrollBar>
#include <QTimer>
#include "ChatMessageWidget.hpp"

#define MAX_NUMBER_OF_ENTRIES 15

using namespace Ui;

ChatWidget::ChatWidget(QWidget *parent) : QScrollArea(parent)
{
    setWidgetResizable(true);
    _container = new QWidget();
    this->setWidget(_container);
    _layout = new QVBoxLayout(_container);
    _layout->setMargin(10);
}

ChatWidget::~ChatWidget()
{
}

void ChatWidget::AddEntry(ChatMessage message, bool botMessage)
{
    Q_UNUSED(botMessage);
    if (_layout->count() == MAX_NUMBER_OF_ENTRIES)
    {
        QLayoutItem* entry;
        if ((entry = _layout->takeAt(0)) != 0)
        {
            delete entry->widget();
            delete entry;
        }
    }
    ChatMessageWidget* newEntry = new ChatMessageWidget("");
    newEntry->insertHtml("<b>" + message.GetColor() + message.GetAuthor() + " | "
                         + message.GetTimeStamp() + "</b></font><br>"
                         + message.GetMessage());
    _layout->addWidget(newEntry);
    if (verticalScrollBar()->value() == verticalScrollBar()->maximum())
    {
        QTimer::singleShot(100, this, SLOT(_AutoScrollDown()));
    }
}

void ChatWidget::_AutoScrollDown()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}
