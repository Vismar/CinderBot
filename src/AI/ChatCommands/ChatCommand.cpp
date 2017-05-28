/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ChatCommand.hpp"
#include <Utils/UserData/UserData.hpp>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

ChatCommand::~ChatCommand() { }

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
    case Price:
        sectionName = "Price";
        break;
    case Covenant:
        sectionName = "Covenant";
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
    _cooldown.setHMS(0, 0, 0, 0);
    _moderatorOnly = false;
    _price = 0;
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
            // If we found a price section, should try to get value
            else if (xmlReader.name() == GetSectionName(Price))
            {
                QString price = xmlReader.readElementText();
                int tempPrice = price.toInt();
                // Check price value after converting it from strign
                if (tempPrice >= 0)
                {
                    _price = tempPrice;
                }
            }
            // If we found covenant section, try to get and save it
            else if(xmlReader.name() == GetSectionName(Covenant))
            {
                _covenant = xmlReader.readElementText();
            }
        }
    }

    return (!_name.isEmpty() && !_answers.isEmpty());
}

///////////////////////////////////////////////////////////////////////////

void ChatCommand::Initialize() { }

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
    if (message.GetMessage().toLower().contains(_name))
    {
        bool covenantIsOk = true;
        //Check covenant
        QString userCovenantName = UD_GET_PARAM(message.GetRealName(), UDP_Covenant);
        if ((!_covenant.isEmpty()) && (_covenant != userCovenantName))
        {
            covenantIsOk = false;
        }
        // Get time when command can be executed
        QDateTime timeToUse = _lastTimeUsed.addMSecs(QTime(0,0,0,0).msecsTo(_cooldown));
        // Compare time when command can be used and time when command trying to be executed
        if (covenantIsOk && (timeToUse < QDateTime::currentDateTime()))
        {
            // Get user currency value
            QString tempUserCurrency = UD_GET_PARAM(message.GetRealName() ,UDP_Currency);
            int userCurrency = tempUserCurrency.toInt();
            // Set user currency value to 0 if converting was failed
            if (userCurrency < 0)
            {
                userCurrency = 0;
            }
            // If user have enough of currency to execute command, try to execute it
            if (userCurrency >= _price)
            {
                userCurrency -= _price;
                QString newUserCurrencyValue = QString::number(userCurrency);
                UD_UPDATE(message.GetRealName(), UDP_Currency, newUserCurrencyValue);
                bool returnAnswer(false);
                // Check if command only for moderators
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
                // If all conditions were passed, get random answer
                if (returnAnswer)
                {
                    // Save time of exection
                    _lastTimeUsed = QDateTime::currentDateTime();
                    // Get random id for answer
                    int id = qrand() % _answers.size();
                    // Set returning answer
                    answer = _answers.at(id);
                }
            }
            // Notify user that he/she do not have enough currency
            else
            {
                answer = "Not enough currency! @";
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
