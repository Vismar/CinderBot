/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AdminCovCmdDeleteCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "utils/Database/CustomCommandDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

enum AnswersId
{
    Description = 0,
    CmdDeleted,
    AnswerDeleted,
    NotLeader,
    NoCovenant,
    NoCmd,
    NoAnswer
};

///////////////////////////////////////////////////////////////////////////

AdminCovCmdDeleteCommand::AdminCovCmdDeleteCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdDeleteCommand::Initialize()
{
    _name = "!cov_cmd_delete";
    _regExpName.setPattern("name=(?<name>.+?);");
    _regExpId.setPattern("id=(?<id>[0-9]+?);");

    // Add answers
    _answers.push_back("@. This command can take 2 parameters: name and id. "
                       "Use of parameter looks like this: name=value; id=number;. "
                       "Do not forget ';'. If you want to delete your command specify its name and nothing more. "
                       "If you want to delete certain answer, then specify command name and id of answer. "
                       "To get id of you command answer, please use !cov_cmd_answer.");
    _answers.push_back("Command 'CMD_NAME' was deleted, @.");
    _answers.push_back("Answer ANSWER_ID  for command 'CMD_NAME' was deleted, @.");
    _answers.push_back("You are not leader of your covenant, @.");
    _answers.push_back("You are not in covenant yet, @.");
    _answers.push_back("No such command in your covenant, @.");
    _answers.push_back("No such answer for your command, @.");
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdDeleteCommand::_GetAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    bool noCommand(false);
    QString covenant = UD_GET_PARAM(message.GetRealName(), UDP_Covenant);
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader, CmdSlots", QString("Name = '%1'").arg(covenant));
        if ((query != nullptr) && (query->first()))
        {
            // If user is leader, than proceed
            if (query->value("Leader").toString() == message.GetRealName())
            {
                QRegularExpressionMatch matchName = _regExpName.match(message.GetMessage());
                QRegularExpressionMatch matchId = _regExpId.match(message.GetMessage());
                CustomCommandDBHelper &DBHelper = CustomCommandDBHelper::Instance();

                // If user specified "name" parameter try to find command and "id" param
                if (matchName.hasMatch())
                {
                    QString cmdName = matchName.captured("name");
                    // Check if that command exist
                    // If command exist, check if "id" param was specified
                    if (DBHelper.CommandExist(cmdName, covenant))
                    {
                        // If paramater "id" was specified, try to find this id in list of ids of answers of specified command
                        if (matchId.hasMatch())
                        {
                            // Get array of command answer ids and check if specified id is in that array
                            QVector<int> cmdAnswerIds = DBHelper.GetAnswers(CmdType::CovenantCmd, cmdName);
                            int cmdId = matchId.captured("id").toInt();

                            // If specified id exist, then proceed
                            if (cmdAnswerIds.contains(cmdId))
                            {
                                // Delete answer
                                DBHelper.DeleteAnswer(CmdType::CovenantCmd, cmdId);

                                // Set answer
                                QString temp = _answers.at(AnswerDeleted);
                                temp.replace("ANSWER_ID", QString::number(cmdId));
                                temp.replace("CMD_NAME", cmdName);
                                answer.AddAnswer(temp);
                            }
                            else
                            {
                                answer.AddAnswer(_answers.at(NoAnswer));
                            }
                        }
                        // If parameter "id" was not specified, just delete command
                        else
                        {
                            // Delete command
                            DBHelper.DeleteCommand(CmdType::CovenantCmd, cmdName);

                            // Set answer
                            QString temp = _answers.at(CmdDeleted);
                            temp.replace("CMD_NAME", cmdName);
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

void AdminCovCmdDeleteCommand::_GetRandomAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
