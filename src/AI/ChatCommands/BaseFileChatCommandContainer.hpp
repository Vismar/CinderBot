#pragma once
#include "../../Chat/ChatMessage.hpp"
#include <QString>
#include <QVector>
#include <QTime>

/*!
 * Class BFChatCommand
 * Is a simple container that give ability to store command with multiple answers and cooldown.
 */
class BFChatCommand
{
private:
    /*! Command name */
    QString          _command;
    /*! List of answers */
    QVector<QString> _answers;
    /*! Time, when command was executed for a last time */
    QTime            _lastTimeUsed;
    /*! Command cooldown */
    QTime            _cooldown;

public:
    /*!
     * Initialize command with specified data
     * \param(IN) command - command name
     * \param(IN) answers - list of answers
     * \param(IN) cooldown - command cooldown
     */
    void InitializeCommand(const QString& command, const QVector<QString>& answers, QTime cooldown);
    /*!
     * Try to execute command and return random answer
     * \param(IN) message - chat message
     * \return answer, if command was exectued. If command was not executed, string will be empty.
     */
    QString GetRandomAnswer(const ChatMessage& message);
};
