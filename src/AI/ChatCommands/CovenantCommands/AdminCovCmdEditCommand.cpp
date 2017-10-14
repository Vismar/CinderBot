/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "AdminCovCmdEditCommand.hpp"
#include "Utils//Database/UserDataDBHelper.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"

using namespace Command::CovenantCmd;
using namespace Utils::Database;

enum Answer
{
    Description = 0,
    CmdCreated,
    CmdUpdated,
    NotLeader,
    NoCovenant,
    NoCmd,
    NoCmdSlots
};

///////////////////////////////////////////////////////////////////////////

AdminCovCmdEditCommand::AdminCovCmdEditCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdEditCommand::Initialize()
{
    _name = "!cov_cmd_edit";
    _regExpName.setPattern("name=(?<name>.+?);");
    _regExpCooldown.setPattern("cooldown=(?<cooldown>[0-9]*[0-9]:[0-9]*[0-9]:[0-9]*[0-9]);");
    _regExpPrice.setPattern("price=(?<price>[0-9]+?);");

    // Add answers
    _answers.push_back("@. This command can take 3 parameters: name, cooldown, and price. "
                       "Use of parameter looks like this: name=value; cooldown=h:m:s; price=number;. "
                       "Do not forget ';'. If you want to edit your command specify its name and specify "
                       "the parameter that you want to change (you can specify both of them in 1 message). "
                       "If you will do the same, but specified command does not exist "
                       "then new command will be created if your covenant has free command slots.");
    _answers.push_back("Command 'CMD_NAME' was created, @.");
    _answers.push_back("Command 'CMD_NAME' was updated, @.");
    _answers.push_back("You are not leader of your covenant, @.");
    _answers.push_back("You are not in covenant yet, @.");
    _answers.push_back("No such command in your covenant, @.");
    _answers.push_back("Your covenant does not have enough command slots to create a new command, @.");
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdEditCommand::_GetAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    bool noCommand(false);
    QString covenant = UserDataDBHelper::GetUserParameter(UserDataParameter::Covenant, message.GetUserID()).toString();
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
                QRegularExpressionMatch matchCooldown = _regExpCooldown.match(message.GetMessage());
                QRegularExpressionMatch matchPrice = _regExpPrice.match(message.GetMessage());
                int numberOfCommands = CustomCommandDBHelper::GetNumberOfCommands(CmdType::CovenantCmd, covenant);

                // If all parameters were specified
                if (matchName.hasMatch() && matchCooldown.hasMatch() && matchPrice.hasMatch())
                {
                    _HandleThreeParams(matchName.captured("name"), matchCooldown.captured("cooldown"), matchPrice.captured("price"),
                                       covenant, answer, numberOfCommands, query->value("CmdSlots").toInt());
                }
                // If not all params were specified
                else if (matchName.hasMatch())
                {
                    bool cmdExist = CustomCommandDBHelper::CommandExist(matchName.captured("name"), covenant);
                    // Try to update cooldown value
                    if (matchCooldown.hasMatch())
                    {
                        _HandleCooldown(matchName.captured("name"), matchCooldown.captured("cooldown"),
                                        covenant, answer, numberOfCommands, query->value("CmdSlots").toInt());
                    }
                    // Try to update price value
                    else if (matchPrice.hasMatch())
                    {
                        _HandlePrice(matchName.captured("name"), matchPrice.captured("price"),
                                     covenant, answer, numberOfCommands, query->value("CmdSlots").toInt());
                    }
                    else if (!cmdExist && (numberOfCommands < query->value("CmdSlots").toInt()))
                    {
                        if (_CreateCommand(matchName.captured("name"), covenant))
                        {
                            QString temp = _answers.at(CmdCreated);
                            temp.replace("CMD_NAME", matchName.captured("name"));
                            answer.AddAnswer(temp);
                        }
                    }
                }
                // If "name" parameter was not specified, display info about command
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

void AdminCovCmdEditCommand::_GetRandomAnswer(const ChatMessage& message, ChatAnswer& answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdEditCommand::_HandleThreeParams(const QString &cmdName, const QString &cooldown, const QString &price, const QString &covenant, 
                                                ChatAnswer &answer, int numberOfCommands, int cmdLost)
{
    QString temp;
    // Check if command exist
    if (CustomCommandDBHelper::CommandExist(cmdName, covenant))
    {
        // Update commands
        _UpdateCommand(cmdName, cooldown, price);

        temp = _answers.at(CmdUpdated);
        temp.replace("CMD_NAME", cmdName);
        answer.AddAnswer(temp);
    }
    else
    {
        // Check available slots
        if (numberOfCommands < cmdLost)
        {
            // Try to create command
            if (_CreateCommand(cmdName, covenant, cooldown, price))
            {
                temp = _answers.at(CmdCreated);
                temp.replace("CMD_NAME", cmdName);
                answer.AddAnswer(temp);
            }
        }
        // If covenant do not have slots, notify about it
        else
        {
            answer.AddAnswer(_answers.at(NoCmdSlots));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdEditCommand::_HandleCooldown(const QString& cmdName, const QString& cooldown, const QString& covenant, 
                                             ChatAnswer& answer, int numberOfCommands, int cmdSlots)
{
    QString temp;
    bool cmdExist = CustomCommandDBHelper::CommandExist(cmdName, covenant);

    if (cmdExist)
    {
        _UpdateCommand(cmdName, cooldown);

        temp = _answers.at(CmdUpdated);
        temp.replace("CMD_NAME", cmdName);
        answer.AddAnswer(temp);
    }
    // If command does not exist, create it
    else if (numberOfCommands < cmdSlots)
    {
        if (_CreateCommand(cmdName, covenant, cooldown))
        {
            temp = _answers.at(CmdCreated);
            temp.replace("CMD_NAME", cmdName);
            answer.AddAnswer(temp);
        }
    }
    else
    {
        answer.AddAnswer(_answers.at(NoCmdSlots));
    }
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdEditCommand::_HandlePrice(const QString& cmdName, const QString& price, const QString& covenant,
                                          ChatAnswer& answer, int numberOfCommands, int cmdSlots)
{
    QString temp;
    bool cmdExist = CustomCommandDBHelper::CommandExist(cmdName, covenant);

    // If command exist, update it
    if (cmdExist)
    {
        _UpdateCommand(cmdName, "", price);

        temp = _answers.at(CmdUpdated);
        temp.replace("CMD_NAME", cmdName);
        answer.AddAnswer(temp);
    }
    // If command does not exist, create it
    else if (numberOfCommands < cmdSlots)
    {
        if (_CreateCommand(cmdName, covenant, "", price))
        {
            temp = _answers.at(CmdCreated);
            temp.replace("CMD_NAME", cmdName);
            answer.AddAnswer(temp);
        }
    }
    else
    {
        answer.AddAnswer(_answers.at(NoCmdSlots));
    }
}

///////////////////////////////////////////////////////////////////////////

void AdminCovCmdEditCommand::_UpdateCommand(const QString& cmdName, const QString& cooldown, const QString& price)
{
    if (!cooldown.isEmpty())
    {
        CustomCommandDBHelper::Instance().SetParameter(CmdType::CovenantCmd, cmdName,
                                                       CustomCmdParameter::Cooldown, cooldown);
    }
    if (!price.isEmpty())
    {
        CustomCommandDBHelper::Instance().SetParameter(CmdType::CovenantCmd, cmdName,
                                                       CustomCmdParameter::Price, price);
    }
}

///////////////////////////////////////////////////////////////////////////

bool AdminCovCmdEditCommand::_CreateCommand(const QString& cmdName, const QString &covenant, const QString &cooldown, const QString& price) const
{
    CmdParams cmdParams;
    cmdParams.Covenant = covenant;
    if (!cooldown.isEmpty())
    {
        cmdParams.Cooldown = QTime::fromString(cooldown, "h:m:s");
    }
    if (!price.isEmpty())
    {
        cmdParams.Price = price.toInt();
    }

    return CustomCommandDBHelper::Instance().CreateCommand(CmdType::CovenantCmd, cmdName, cmdParams);
}

///////////////////////////////////////////////////////////////////////////
