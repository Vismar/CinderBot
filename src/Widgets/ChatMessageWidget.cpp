#include "ChatMessageWidget.hpp"

using namespace Ui;

ChatMessageWidget::ChatMessageWidget(const QString& text, QWidget* parent) : QTextEdit(text, parent)
{
    // Make it look and feel like an ordinary label
    setReadOnly(true);
    setFrameStyle(QFrame::Box);
    QPalette pal = palette();
    pal.setColor(QPalette::Base, Qt::transparent);
    setPalette(pal);

    // Wrap anywhere, adjust minimum height on the fly
    setLineWrapMode(QTextEdit::WidgetWidth);
    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    connect(document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            this, SLOT(_AdjustMinimumSize(QSizeF)));
}

void ChatMessageWidget::_AdjustMinimumSize(const QSizeF& size)
{
    setMinimumHeight(size.height() + 2 * frameWidth());
}
