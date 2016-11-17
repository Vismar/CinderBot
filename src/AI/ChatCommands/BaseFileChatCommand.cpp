#include "BaseFileChatCommand.hpp"
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QTime>

#define BFCC_FILE_NAME       "./data/config/commands.xml"
#define BFCC_SECTION_COMMAND "Command"
#define BFCC_SECTION_NAME    "Name"
#define BFCC_SECTION_ANSWER  "Answer"
#define BFCC_SYMBOL_AUTHOR   "@"

bool BaseFileChatCommand::Initialize()
{
    bool result(true);
    QFile commandsFile(BFCC_FILE_NAME);
    if (commandsFile.open(QIODevice::ReadOnly))
    {
        _xmlReader.setDevice(&commandsFile);

        while (!_xmlReader.atEnd())
        {
            _xmlReader.readNext();
            if (_xmlReader.isStartElement())
            {
                if (_xmlReader.name() == BFCC_SECTION_COMMAND)
                {
                    _ReadCommand();
                }
            }
        }
    }
    else
    {
        result = false;
    }
    commandsFile.close();

    return result;
}

bool BaseFileChatCommand::GetAnswer(ChatMessage& message, QString& answer)
{
    bool result(false);
    for (int i = 0; i < _commands.size(); ++i)
    {
        if (message.GetMessage().contains(_commands.at(i).first))
        {
            result = true;
            int k = qrand() % _commands.at(i).second.size();
            answer = _commands.at(i).second.at(k);
            if (answer.contains(BFCC_SYMBOL_AUTHOR))
            {
                answer.insert(answer.indexOf(BFCC_SYMBOL_AUTHOR) + 1,
                              message.GetAuthor());
            }
            break;
        }
    }

    return result;
}

void BaseFileChatCommand::_ReadCommand()
{
    QString name;
    QVector<QString> answers;
    while (!_xmlReader.atEnd())
    {
        _xmlReader.readNext();
        if (_xmlReader.isEndElement())
        {
            if (_xmlReader.name() == BFCC_SECTION_COMMAND)
            {
                break;
            }
        }
        if (_xmlReader.isStartElement())
        {
            if((_xmlReader.name() == BFCC_SECTION_NAME) && (name.isEmpty()))
            {
                name = _xmlReader.readElementText();
            }
            else if (_xmlReader.name() == BFCC_SECTION_ANSWER)
            {
                QString answer = _xmlReader.readElementText();
                answer.replace("\n", "");
                answer.replace("\t", "");
                if (!answer.isEmpty())
                {
                    answers.push_back(answer);
                }
            }
        }
    }
    if (!name.isEmpty() && !answers.isEmpty())
    {
        _commands.push_back(QPair<QString, QVector<QString> >(name, answers));
    }
}
