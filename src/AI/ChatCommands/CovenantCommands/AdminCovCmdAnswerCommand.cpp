/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AdminCovCmdAnswerCommand.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/Database/RPG/CovenantDBHelper.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

enum AnswersId
{
    Description = 0,
    AnswerIds,
    Answer,
    AnswerAdded,
    AnswerUpdated,
    NotLeader,
    NoCovenant,
    NoCmd,
    NoAnswer
};

///////////////////////////////////////////////////////////////////////////

AdminCovCmdAnswerCommand::AdminCovCmdAnswerCommand()
{
    _name = "!cov_cmd_answer";
    _regExpName.setPattern("name=(?<name>.+?);");
    _regExpId.setPattern("id=(?<id>[0-9]+?);");
    _regExpAnswer.setPattern("answer=(?<answer>.+?);");

    // Add answers
    _answers.push_back("@. This command can take 3 parameters: name, id and answer. "
                       "Use of parameter looks like this: name=value; id=number; answer=string;. "
                       "Do not forget ';'. To see all answer ids specify command name. "
                       "To see certain answer specify command name and id of the answer. "
                       "To edit an answer, specify command name, id and answer. "
                       "To add an answer, specify command name and answer.");
    _answers.push_back("@, your command have these answer ids: CMD_ANSWER_IDS.");
    _answers.push_back("@, answer = 'ANSWER'.");
    _answers.push_back("New answer was added, @.");
    _answers.push_back("You answer was updated, @.");
    _answers.push_back("You are not leader of your covenant, @.");
    _answers.push_back("You are not in covenant yet, @.");
    _answers.push_back("No such command in your covenant, @.");
    _answers.push_back("No such answer for your command, @.");
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdAnswerCommand::_GetAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    bool noCommand(false);
    QString covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        if (CovenantDBHelper::CheckLeadership(message.GetUserID()))
        {
            QRegularExpressionMatch matchName = _regExpName.match(message.GetMessage());
            QRegularExpressionMatch matchId = _regExpId.match(message.GetMessage());
            QRegularExpressionMatch matchAnswer = _regExpAnswer.match(message.GetMessage());
            CustomCommandDBHelper &DBHelper = CustomCommandDBHelper::Instance();

            // If user specified "name" parameter try to find command
            if (matchName.hasMatch())
            {
                QString cmdName = matchName.captured("name");
                // Check if that command exist
                // If command exist, check other parameters
                if (DBHelper.CommandExist(cmdName, covenant))
                {
                    // If parameter "id" was specified, try to find this id in list of ids of answers of specified command
                    if (matchId.hasMatch())
                    {
                        // Get array of command answer ids and check if specified id is in that array
                        QVector<int> cmdAnswerIds = DBHelper.GetAnswers(CmdType::CovenantCmd, cmdName);
                        int cmdId = matchId.captured("id").toInt();

                        // If specified id exist, then proceed
                        if (cmdAnswerIds.contains(cmdId))
                        {
                            // If we have parameter "answer", then update answer with specified id
                            if (matchAnswer.hasMatch())
                            {
                                // Update answer with specified value
                                DBHelper.EditAnswer(CmdType::CovenantCmd, cmdId, matchAnswer.captured("answer"));

                                // Set answer
                                answer.AddAnswer(_answers.at(AnswerUpdated));
                            }
                            // If parameter "answer" was not specified, then display answer with specified id
                            else
                            {
                                // Get command answer and set it to chat answer
                                QString temp = _answers.at(Answer);
                                temp.replace("ANSWER", DBHelper.GetAnswer(CmdType::CovenantCmd, cmdId));
                                answer.AddAnswer(temp);
                            }
                        }
                        // If id is not contained in grabbed array then notify user that provided id is irrelevant
                        else
                        {
                            answer.AddAnswer(_answers.at(NoAnswer));
                        }
                    }
                    // If parameter "id" was not specified, check parameter "answer"
                    else if (matchAnswer.hasMatch())
                    {
                        // Add new answer
                        DBHelper.AddAnswer(CmdType::CovenantCmd, cmdName, matchAnswer.captured("answer"));

                        // Set chat answer
                        answer.AddAnswer(_answers.at(AnswerAdded));
                    }
                    // if parameter "answer" was not specified, then return list of command answer ids
                    else
                    {
                        // Grab ids to display
                        QVector<int> commandAnswerIds = DBHelper.GetAnswers(CmdType::CovenantCmd, cmdName);
                        QString ids;
                        for (int i = 0; i < commandAnswerIds.size(); ++i)
                        {
                            if (i > 0)
                            {
                                ids.append(", ");
                            }
                            ids.append(QString::number(commandAnswerIds.at(i)));
                        }

                        QString temp = _answers.at(AnswerIds);
                        temp.replace("CMD_ANSWER_IDS", ids);
                        answer.AddAnswer(temp);
                    }
                }
                // If command was not found, notify user about it
                else
                {
                    answer.AddAnswer(_answers.at(NoCmd));
                }
            }
            // If parameter "name" was not specified, display info about command
            else
            {
                answer.AddAnswer(_answers.at(Description));
            }
        }
        // If user not leader, say it
        else
        {
            answer.AddAnswer(_answers.at(NotLeader));
        }
    }
    // If user is not a member of any covenant
    else
    {
        answer.AddAnswer(_answers.at(NoCovenant));
    }

    if (noCommand)
    {
        answer.AddAnswer(_answers.at(NoCmd));
    }
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdAnswerCommand::_GetRandomAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
