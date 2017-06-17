/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ChatMessageWidget.hpp"
#include <QAbstractTextDocumentLayout>

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

ChatMessageWidget::ChatMessageWidget(const QString &text, QWidget* parent) : QTextEdit(text, parent)
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

///////////////////////////////////////////////////////////////////////////

void ChatMessageWidget::_AdjustMinimumSize(const QSizeF &size)
{
    // Sets minimum size of widget, prevents it from squeezing
    setMinimumHeight(size.height() + 2 * frameWidth());
    // Sets maximum size of widget, prevents it from stretching
    setMaximumHeight(size.height() + 2 * frameWidth());
}

///////////////////////////////////////////////////////////////////////////
