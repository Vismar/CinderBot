#include "ChatCommand.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

QString ChatCommand::GetSectionName(CmdSection cmdSection)
{
    QString sectionName;

    switch (cmdSection)
    {
    case Start:
        sectionName = "Command";
        break;
    case Name:
        sectionName = "Name";
        break;
    case Answer:
        sectionName = "Answer";
        break;
    case Cooldown:
        sectionName = "Cooldown";
        break;
    case ModOnly:
        sectionName = "ModeratorOnly";
        break;
    }

    return sectionName;
}

///////////////////////////////////////////////////////////////////////////

void ChatCommand::_Clear()
{
    _name.clear();
    _answers.clear();
    _answers.squeeze();
    _lastTimeUsed.setHMS(0, 0, 0, 0);
    _cooldown.setHMS(0, 0, 0, 0);
    _moderatorOnly = false;
}

///////////////////////////////////////////////////////////////////////////

void ChatCommand::_AddAuthorName(QString& answer, const QString& author)
{
    if (answer.contains("@"))
    {
        answer.insert(answer.indexOf("@") + 1, author);
    }
}

///////////////////////////////////////////////////////////////////////////

bool ChatCommand::Initialize(QXmlStreamReader& xmlReader)
{
    _Clear();
    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        // If end of command section was founded, break loop
        if (xmlReader.isEndElement())
        {
            if (xmlReader.name() == GetSectionName(Start))
            {
                break;
            }
        }
        if (xmlReader.isStartElement())
        {
            // If we found a command name section for the first time, save it
            if((xmlReader.name() == GetSectionName(Name)) && (_name.isEmpty()))
            {
                _name = xmlReader.readElementText();
            }
            // If we found a cooldown section for the first time, save it
            else if (xmlReader.name() == GetSectionName(Cooldown))
            {
                QString textCooldown = xmlReader.readElementText();
                _cooldown = QTime::fromString(textCooldown, "h:m:s:z");
            }
            // If we found modonly section for the fiest time, save it
            else if (xmlReader.name() == GetSectionName(ModOnly))
            {
                QString textModOnly = xmlReader.readElementText();
                textModOnly.replace(" ", "");
                textModOnly.replace("\n", "");
                textModOnly.replace("\t", "");
                if (textModOnly.toLower() == "true")
                {
                    _moderatorOnly = true;
                }
            }
            // If we found a answer section, should try to get text
            else if (xmlReader.name() == GetSectionName(Answer))
            {
                // Try to get text
                QString answer = xmlReader.readElementText();
                // Remove "new line" and "tab" symbols
                answer.replace("\n", "");
                answer.replace("\t", "");
                // If result text is not empty, add to answers list
                if (!answer.isEmpty())
                {
                    _answers.push_back(answer);
                }
            }
        }
    }

    return (!_name.isEmpty() && !_answers.isEmpty());
}

///////////////////////////////////////////////////////////////////////////

bool ChatCommand::IsInititalized() const
{
    return (!_name.isEmpty() && !_answers.isEmpty());
}

///////////////////////////////////////////////////////////////////////////

QString ChatCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    // Check if message contains command
    if (message.GetMessage().contains(_name))
    {
        // Get time when command can be executed
        QTime timeToUse(_lastTimeUsed.hour() + _cooldown.hour(),
                         _lastTimeUsed.minute() + _cooldown.minute(),
                         _lastTimeUsed.second() + _cooldown.second(),
                         _lastTimeUsed.msec() + _cooldown.msec());
        // Compare time when command can be used and time when command trying to be executed
        if (timeToUse < QTime::currentTime())
        {
            bool returnAnswer(false);
            if (_moderatorOnly)
            {
                if (message.IsModerator())
                {
                    returnAnswer = true;
                }
            }
            else
            {
                returnAnswer = true;
            }
            if (returnAnswer)
            {
                // Save time of exection
                _lastTimeUsed = QTime::currentTime();
                // Get random id for answer
                int id = qrand() % _answers.size();
                // Set returning answer
                answer = _answers.at(id);
            }
        }
    }
    if (!answer.isEmpty())
    {
        _AddAuthorName(answer, message.GetAuthor());
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////
