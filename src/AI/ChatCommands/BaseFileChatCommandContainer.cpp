#include "BaseFileChatCommandContainer.hpp"

///////////////////////////////////////////////////////////////////////////

void BFChatCommand::InitializeCommand(const QString& command, const QVector<QString>& answers, QTime cooldown)
{
    _command = command;
    _answers = answers;
    _cooldown = cooldown;
    // Set it to absolute zero
    _lastTimeUsed.setHMS(0, 0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////

QString BFChatCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    // Check if message contains command
    if (message.GetMessage().contains(_command))
    {
        // Get time when command can be executed
        QTime timeToUse(_lastTimeUsed.hour() + _cooldown.hour(),
                         _lastTimeUsed.minute() + _cooldown.minute(),
                         _lastTimeUsed.second() + _cooldown.second(),
                         _lastTimeUsed.msec() + _cooldown.msec());
        // Compare time when command can be used and time when command trying to be executed
        if (timeToUse < QTime::currentTime())
        {
            // Save time of exection
            _lastTimeUsed = QTime::currentTime();
            // Get random id for answer
            int id = qrand() % _answers.size();
            // Set returning answer
            answer = _answers.at(id);
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
