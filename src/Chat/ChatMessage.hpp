#pragma once
#include <QString>

#define SYSTEM_MESSAGE    "System Message"
#define SYSTEM_MESSAGE_CR "Connected to the room!"

enum MessageType
{
    Undefined = -1,
    INFO,
    PING,
    PONG,
    LOGIN_OK,
    USERSTATE,
    PRIVMSG
};

class ChatMessage
{
private:
    QString _author;
    QString _color;
    QString _timeStamp;
    QString _message;
    bool    _isModerator;

    bool _IsNetworkMsg(const QString& message) const;
    bool _IsPingCommand(const QString& message) const;
    bool _IsPongCommand(const QString& message) const;
    int  _IsIrcComand(const QString& message) const;
    bool _IsConnectedToRoom(const QString& message) const;
    bool _IsUserState(const QString& message) const;
    bool _IsPrivMsg(const QString& message) const;

    void _SetTimeStamp();

public:
    const QString& GetAuthor() const;
    const QString& GetColor() const;
    const QString& GetTimeStamp() const;
    const QString& GetMessage() const;
    bool           IsModerator() const;

    void SetAuthor(const QString& author);
    void SetColor(const QString& color);
    void SetMessage(const QString& message);

    MessageType ParseRawMessage(const QString& message);
};
