#pragma once
#include <QTextEdit>

namespace Ui
{

class ChatMessageWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit ChatMessageWidget(const QString& text, QWidget* parent = 0);

private slots:
    void _AdjustMinimumSize(const QSizeF& size);
};

}
