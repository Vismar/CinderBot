#include "ChatWidget.hpp"
#include "ChatMessageWidget.hpp"
#include <QListWidgetItem>
#include <QTimer>
#include <QScrollBar>

#define MAX_NUMBER_OF_ENTRIES 50

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

ChatWidget::ChatWidget(QWidget* parent) : QScrollArea(parent)
{
    // Set params to layout
    setWidgetResizable(true);
    _container = new QWidget();
    this->setWidget(_container);
    _layout = new QVBoxLayout(_container);
    _layout->setAlignment(Qt::AlignTop);
    _layout->setMargin(10);
}

///////////////////////////////////////////////////////////////////////////

ChatWidget::~ChatWidget() {}

///////////////////////////////////////////////////////////////////////////

void ChatWidget::AddEntry(ChatMessage message, bool botMessage)
{
    Q_UNUSED(botMessage);
    // Check if we already have maximum number of entries
    if (_layout->count() == MAX_NUMBER_OF_ENTRIES)
    {
        // Remove oldest entry
        QLayoutItem* entry;
        if ((entry = _layout->takeAt(0)) != 0)
        {
            delete entry->widget();
            delete entry;
        }
    }
    // Add new entry
    ChatMessageWidget* newEntry = new ChatMessageWidget("");

    QString imageString;
    // Check if author of message is moderator
    if (message.IsModerator())
    {
        // Get font size and multiplay by 2 to change image size
        QString stringFontSize = QString::number(newEntry->font().pointSize() * 2);
        // Append moderator icon before author name
        QString resourceName;
        if (message.IsBroadcaster())
        {
            resourceName = "broadcasterIcon.ico";
        }
        else
        {
            resourceName = "modIcon.ico";
        }
        imageString = QString("<img src=':/Resources/Icons/%1' height='%2' width='%2'>  ").arg(resourceName).arg(stringFontSize);
    }
    newEntry->insertHtml(imageString + "<b>" + message.GetColor() + message.GetAuthor() + " | "
                         + message.GetTimeStamp() + "</b></font><hr>"
                         + message.GetMessage());
    _layout->addWidget(newEntry);
    // If scroll area at the bottom, it should be scrolled down, otherwise it will go up.
    if (verticalScrollBar()->value() == verticalScrollBar()->maximum())
    {
        // Because of delay we should wait a little bit to scroll
        QTimer::singleShot(100, this, SLOT(_AutoScrollDown()));
    }
}

///////////////////////////////////////////////////////////////////////////

void ChatWidget::_AutoScrollDown()
{
    // Scroll to the botton of scroll area
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

///////////////////////////////////////////////////////////////////////////
