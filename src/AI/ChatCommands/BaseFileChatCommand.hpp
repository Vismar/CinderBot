#pragma once
#include "BaseChatCommand.hpp"
#include <QPair>
#include <QString>
#include <QVector>

class BaseFileChatCommand : public BaseChatCommand
{
protected:
    QVector<QPair<QString, QVector<QString> > > _commands;

public:
    bool Initialize();
    bool GetAnswer(ChatMessage& message, QString& answer);
};
