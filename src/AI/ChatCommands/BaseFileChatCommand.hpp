#pragma once
#include "BaseChatCommand.hpp"
#include <QPair>
#include <QString>
#include <QVector>
#include <QXmlStreamReader>

class BaseFileChatCommand : public BaseChatCommand
{
public:
    bool Initialize();
    bool GetAnswer(ChatMessage& message, QString& answer);

protected:
    QVector<QPair<QString, QVector<QString> > > _commands;
    QXmlStreamReader _xmlReader;

private:
    void _ReadCommand();
};
