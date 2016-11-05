#include "BaseFileChatCommand.hpp"
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QTime>

#define BASE_FILE_COMMAND_PATH     "./data/data/"
#define BASE_FILE_COMMAND_NAME     "baseFile.data"
#define BASE_FILE_COMMAND_START    "#"
#define BASE_FILE_COMMAND_ANSWER_S "["
#define BASE_FILE_COMMAND_ANSWER_E "]"
#define BASE_FILE_COMMAND_AUTHOR   "@"

bool BaseFileChatCommand::Initialize()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    bool result(false);
    QVector<QString> answers;
    QString line;
    QString commandName;
    QString answer;
    QString fileName = BASE_FILE_COMMAND_PATH;
    fileName.append(BASE_FILE_COMMAND_NAME);
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            answers.clear();
            line = in.readLine();
            int positionStart, positionEnd(-1);
            positionStart = line.indexOf(BASE_FILE_COMMAND_START);
            if (positionStart != -1)
            {
                positionEnd = line.indexOf(" ");
                if (positionEnd != -1)
                {
                    commandName = line.mid(positionStart, positionEnd - positionStart);
                    positionStart = line.indexOf(BASE_FILE_COMMAND_ANSWER_S,
                                                 positionEnd);
                    positionEnd = line.indexOf(BASE_FILE_COMMAND_ANSWER_E,
                                               positionStart);
                    while ((positionStart != -1) && (positionEnd != -1))
                    {
                        answer = line.mid(positionStart + 1,
                                          positionEnd - positionStart - 1);
                        answers.push_back(answer);
                        positionStart = line.indexOf(BASE_FILE_COMMAND_ANSWER_S,
                                                     positionEnd);
                        positionEnd = line.indexOf(BASE_FILE_COMMAND_ANSWER_E,
                                                   positionStart);
                    }
                    if (answers.size() > 0)
                    {
                        _commands.push_back(QPair<QString, QVector<QString> >(commandName, answers));
                        result = true;
                    }
                }
            }
        }
    }
    file.close();

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
            if (answer.contains(BASE_FILE_COMMAND_AUTHOR))
            {
                answer.insert(answer.indexOf(BASE_FILE_COMMAND_AUTHOR) + 1,
                              message.GetAuthor());
            }
            break;
        }
    }

    return result;
}
