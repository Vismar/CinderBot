#pragma once
#include <QObject>
#include <QVector>
#include <QStringList>
#include "../Chat/ChatMessage.hpp"
#include "./ChatCommands/BaseChatCommand.hpp"

class BotAI : public QObject
{
    Q_OBJECT
private:
    QVector<BaseChatCommand*> _chatCommands;
    QStringList               _ignoreList;

    bool _CheckIgnoreList(const QString& userName);
    void _UpdateUserData(const ChatMessage& message);
    void _IncrementMessageCounter(const QString& userName);
    void _AddCurrency(const QString& userName, const int value);

public:
    explicit BotAI(QObject* parent = 0);
    ~BotAI() {}

signals:
    void NewBotMessage(QString message);

public slots:
    void ReadNewMessage(ChatMessage message, bool botMessage);
};
