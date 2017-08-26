/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AdminCovCmdCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "utils/Database/CustomCommandDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

#define MSG_CMD_INFO    0
#define MSG_CMD_LIST    1
#define MSG_CMD_NO_CMD  2
#define MSG_NOT_LEADER  3
#define MSG_NO_COVENANT 4
#define MSG_NO_SUCH_CMD 5

///////////////////////////////////////////////////////////////////////////

AdminCovCmdCommand::AdminCovCmdCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdCommand::Initialize()
{
    _name = "!cov_cmd";
    _regExpName.setPattern("name=(?<name>.+);");
    // Add answers
    _answers.push_back("Command 'CMD_NAME': Cooldown - COOLDOWN; Price - PRICE; Answers - ANSWERS.");
    _answers.push_back("Commands of your covenant 'COV_NAME': COMMAND_LIST. To see info about specific command add this param: name=COMMAND_NAME; . Where COMMAND_NAME is a name of certain command.");
    _answers.push_back("Your covenant 'COV_NAME' do not have any commands yet, @.");
    _answers.push_back("You are not leader of your covenant, @.");
    _answers.push_back("You are not in covenant yet, @.");
    _answers.push_back("No such command in your covenant, @.");
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdCommand::_GetAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    QString covenant = UD_GET_PARAM(message.GetRealName(), UDP_Covenant);
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
        if ((query != nullptr) && (query->first()))
        {
            // If user is leader
            if (query->value("Leader").toString() == message.GetRealName())
            {
                QString temp;
                // Check match with regular expression
                QRegularExpressionMatch match = _regExpName.match(message.GetMessage());
                // If parameter was specified, display info about command.
                if (match.hasMatch())
                {
                    // Check if command exist
                    if (CustomCommandDBHelper::Instance().CommandExist(match.captured("name")))
                    {
                        // Get number of answers
                        int numberOfAnswers = CustomCommandDBHelper::Instance().GetAnswers(CmdType::CovenantCmd, match.captured("name")).size();
                        // Get params
                        CmdParams params = CustomCommandDBHelper::Instance().GetAllParams(CmdType::CovenantCmd, match.captured("name"));
                        temp = _answers.at(MSG_CMD_INFO);
                        // replace placeholders
                        temp.replace("CMD_NAME", match.captured("name"));
                        temp.replace("COOLDOWN", params.Cooldown.toString("h:m:s"));
                        temp.replace("PRICE", QString::number(params.Price));
                        temp.replace("ANSWERS", QString::number(numberOfAnswers));
                        answer.AddAnswer(temp);
                    }
                    // If command do not exist, notify user about it
                    else
                    {
                        answer.AddAnswer(_answers.at(MSG_NO_SUCH_CMD));
                    }
                }
                // If parameter "name" was not specified, display command list.
                else
                {       
                    // Get command list
                    QStringList commandList = CustomCommandDBHelper::Instance().GetCommandNames(CmdType::CovenantCmd, covenant);
                    if (commandList.size() > 0)
                    {
                        temp = _answers.at(MSG_CMD_LIST);
                        temp.replace("COV_NAME", covenant);
                        QString commands;
                        for (int i = 0; i < commandList.size(); ++i)
                        {
                            if (i > 0)
                            {
                                commands.append(", ");
                            }
                            commands.append(commandList.at(i));
                        }
                        temp.replace("COMMAND_LIST", commands);
                        answer.AddAnswer(temp);
                    }
                    // If covenant do not have any command yet
                    else
                    {
                        temp = _answers.at(MSG_CMD_NO_CMD);
                        temp.replace("COV_NAME", covenant);
                        answer.AddAnswer(temp);
                    }
                }
            }
            // If user not leader, say it
            else
            {
                answer.AddAnswer(_answers.at(MSG_NOT_LEADER));
            }
        }
    }
    // If user is not a member of any covenant
    else
    {
        answer.AddAnswer(_answers.at(MSG_NO_COVENANT));
    }
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdCommand::_GetRandomAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////