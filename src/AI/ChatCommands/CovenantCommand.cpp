/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CovenantCommand.hpp"
#include <Utils/UserData/UserData.hpp>
#include <Utils/Config/ConfigurationManager.hpp>
#include <Utils/Config/ConfigurationParameters.hpp>
#include <Utils/DatabaseManager.hpp>
#include <QStringList>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

void CovenantCommand::SetCommandType(CovCommandType cmdType)
{
    _cmdType = cmdType;
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

QString CovenantCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    if (message.GetMessage().contains(_name))
    {
        switch (_cmdType)
        {
        case Cov_List:
            answer = _GetAnswerForCovListCmd();
            break;
        case Cov_Join:
            answer = _GetAnswerForCovJoinCmd(message);
            break;
        case Cov_Leave:
            answer = _GetAnswerForCovLeaveCmd(message);
            break;
        case Cov_Disband:
            answer = _GetAnswerForCovDisbandCmd(message);
            break;
        case Cov_Create:
            answer = _GetAnswerForCovCreateCmd(message);
            break;
        case Cov_Rename:
            answer = _GetAnswerForCovRenameCmd(message);
            break;
        default:
            break;
        }
    }
    if (!answer.isEmpty())
    {
        _AddAuthorName(answer, message.GetAuthor());
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

void CovenantCommand::Initialize()
{
    _Clear();
    switch (_cmdType)
    {
    case Cov_List:
        _name = "!cov_list";
        _answers.push_back("Covenants: COV_LIST");
        break;
    case Cov_Join:
        _name = "!cov_join";
        _answers.push_back("You are joining 'COV_NAME', @.");
        _answers.push_back("You are already in that covenant, @!");
        _answers.push_back("Not enough currency, @!");
        _answers.push_back("You are leader of your covenant, @! Please leave before trying to join.");
        break;
    case Cov_Leave:
        _name = "!cov_leave";
        _answers.push_back("You left covenant! Now you are just a viewer, @");
        _answers.push_back("You are leader of covenant, @! Please provide name of the new leader.");
        _answers.push_back("You left covenant, @! New leader of COV_NAME is LEADER_NAME.");
        _answers.push_back("Specified user is not member of your covenant!");
        _answers.push_back("You are not in any covenant yet, @.");
        break;
    case Cov_Disband:
        _name = "!cov_disband";
        _answers.push_back("Your covenant was disbanded, @!");
        _answers.push_back("You are not leader of your covenant, @.");
        _answers.push_back("You are not in any covenant yet, @.");
        break;
    case Cov_Create:
        _name = "!cov_create";
        _answers.push_back("You do not have enought currency, @.");
        _answers.push_back("You are leader of another covenant, @! Leave or disband it and try again.");
        _answers.push_back("Covenant 'COV_NAME' was created. @! Now you are leader of it.");
        _answers.push_back("Please provide name of new covenant, @.");
        break;
    case Cov_Rename:
        _name = "!cov_rename";
        _answers.push_back("You are not leader of your covenant, @!");
        _answers.push_back("Your covenant has been renamed, @.");
        _answers.push_back("You are not in any covenant yet, @.");
        _answers.push_back("Please provide a new name of covenant, @.");
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////

QString CovenantCommand::_GetAnswerForCovListCmd()
{
    QString answer;
    QString covList;
    // Get covenant list
    std::shared_ptr<QSqlQuery> query = DB_SELECT("Covenants", "Name");
    if (query->exec())
    {
        answer = _answers.first();
        while (query->next())
        {
            covList.append(query->value("Name").toString() + ", ");
        }
        if (covList.size() > 0)
        {
            covList = covList.left(covList.size() - 2);
        }
    }
    answer.replace("COV_LIST", covList);

    return answer;
}

///////////////////////////////////////////////////////////////////////////

QString CovenantCommand::_GetAnswerForCovJoinCmd(const ChatMessage& message)
{
    QString answer;
    QString price;
    ConfigurationManager& configMng = ConfigurationManager::Instance();
    UserData& userData = UserData::Instance();
    QString covenant = userData.GetUserDataParam(message.GetRealName(), UDP_Covenant);

    // Check if user is leader of its covenant
    if (covenant != "Viewer")
    {
        std::shared_ptr<QSqlQuery> queryLeader = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
        if (queryLeader->exec())
        {
            queryLeader->first();
            // If user is leader, notify him about it
            if (queryLeader->value("Leader").toString() == message.GetRealName())
            {
                answer = _answers.at(3);
            }
        }
    }
    // If user Viewer or not a leader of its covenant
    if (answer.isEmpty())
    {
        // Get amount of currency which user have
        int currency = userData.GetUserDataParam(message.GetRealName(), UDP_Currency).toInt();
        // Set default price value
        int priceToJoin(100);
        // Try to get param from config manager
        if (configMng.GetStringParam(CFGP_COV_JOIN_PRICE,price))
        {
            priceToJoin = price.toInt();

        }
        // If user have enough currency to join
        if (currency >= priceToJoin)
        {
            // Get covenant list
            QStringList covenants;
            std::shared_ptr<QSqlQuery> query = DB_SELECT("Covenants", "Name");
            if (query->exec())
            {
                while (query->next())
                {
                    covenants.append(query->value("Name").toString());
                }
            }
            // Try to find specified covenant from list of covenants
            for (auto iter = covenants.begin(); iter != covenants.end(); ++iter)
            {
                // Found a overlap
                if (message.GetMessage().contains(*iter))
                {
                    // Check if user already in this covenant
                    if (userData.GetUserDataParam(message.GetRealName(), UDP_Covenant) != *iter)
                    {
                        // Join user to covenant and take currency for it
                        answer = _answers.first();
                        answer.replace("COV_NAME", *iter);
                        userData.UpdateUserData(message.GetRealName(), UDP_Covenant, *iter);
                        userData.UpdateUserData(message.GetRealName(), UDP_Currency, QString::number(currency - priceToJoin));
                    }
                    // User is already in that covenant
                    else
                    {
                        // Second answer
                        answer = _answers.at(1);
                    }
                    break;
                }
            }
        }
        else
        {
            answer = _answers.last();
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

QString CovenantCommand::_GetAnswerForCovLeaveCmd(const ChatMessage& message)
{
    QString answer;
    UserData& userData = UserData::Instance();
    QString covenant = userData.GetUserDataParam(message.GetRealName(), UDP_Covenant);
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        std::shared_ptr<QSqlQuery> query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
        if (query->exec())
        {
            query->first();
            // If user is leader, than try to find specified name of new leader
            if (query->value("Leader").toString() == message.GetRealName())
            {
                // Name of new leader should start with ampersand
                int ampersand = message.GetMessage().indexOf("@");
                if (ampersand > 0)
                {
                    // Try to get name of new leader
                    QString newLeader;
                    int endOfName = _GetEndOfNameFromAmpersand(ampersand, message.GetMessage());
                    newLeader = message.GetMessage().mid(ampersand+1, endOfName-ampersand-1);
                    // If leader name was found, try to use it
                    if (!newLeader.isEmpty())
                    {
                        if (_SetNewLeaderToCovenant(newLeader, message.GetRealName(), covenant))
                        {
                            answer = _answers.at(2);
                            answer.replace("COV_NAME", covenant);
                            answer.replace("LEADER_NAME", QString("@%1").arg(newLeader));
                        }
                        // Notify that user is not member of covenant
                        else
                        {
                            answer = _answers.at(3);
                        }
                    }
                    // Notify that name was not specified
                    else
                    {
                        answer = _answers.at(1);
                    }
                }
                // If message do not contain ampersand, so that means user do not provide name of new leader
                else
                {
                    answer = _answers.at(1);
                }
            }
            // If user is not a leader of covenant, that just change its state
            else
            {
                userData.UpdateUserData(message.GetRealName(), UDP_Covenant, "Viewer");
            }
        }
    }
    // If user is not in any covenant
    else
    {
        answer = _answers.at(4);
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

QString CovenantCommand::_GetAnswerForCovDisbandCmd(const ChatMessage& message)
{
    QString answer;
    UserData& userData = UserData::Instance();
    QString covenant = userData.GetUserDataParam(message.GetRealName(), UDP_Covenant);
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        std::shared_ptr<QSqlQuery> query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
        if (query->exec())
        {
            query->first();
            // If user is leader, than proceed disband of covenant
            if (query->value("Leader").toString() == message.GetRealName())
            {
                // If covenant was disbanded, set covenant field to viewer for all users who was in that covenant
                if (DB_DELETE("Covenants", QString("Name = '%1'").arg(covenant)))
                {
                    std::shared_ptr<QSqlQuery> query = DB_SELECT("UserData", "Name",
                                                                             QString("Covenant = '%1'").arg(covenant));
                    if (query != NULL)
                    {
                        while (query->next())
                        {
                            userData.UpdateUserData(query->value("Name").toString(), UDP_Covenant, "Viewer");
                        }
                    }
                    answer = _answers.at(0);
                }
            }
            // If user not leader, say it
            else
            {
                answer = _answers.at(1);
            }
        }
    }
    // If user is not a member of any covenant
    else
    {
        answer = _answers.at(2);
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

QString CovenantCommand::_GetAnswerForCovCreateCmd(const ChatMessage& message)
{
    QString answer;
    UserData& userData = UserData::Instance();
    QString covenant = userData.GetUserDataParam(message.GetRealName(), UDP_Covenant);
    // Check if user is leader of its covenant
    std::shared_ptr<QSqlQuery> query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
    if (query->exec())
    {
        // If user is in covenant, check its leader
        if (query->first())
        {
            if (query->value("Leader").toString() == message.GetRealName())
            {
                // User is leader of another covenant
                answer = _answers.at(1);
            }
        }
        // If user is not a leader or he not in a covenant, he is allowed to create covenant
        if (answer.isEmpty())
        {
            // Get currency which user have
            QString currency = userData.GetUserDataParam(message.GetRealName(), UDP_Currency);
            // Get amount currency which is needed to create covenant
            QString price;
            if (!ConfigurationManager::Instance().GetStringParam(CFGP_COV_CREATE_PRICE, price))
            {
                price = "2000";
            }
            // Check if user have enough currency
            if (currency.toInt() >= price.toInt())
            {
                // Get covenant name
                QString newCovenant = message.GetMessage().mid(_name.size()+1);
                int covNameLength = ((newCovenant.size()-2) < 0) ? 0 : (newCovenant.size()-2);
                // If covenant name too long, just make it shorter
                if (covNameLength > 50)
                {
                    covNameLength = 50;
                }
                newCovenant = newCovenant.left(covNameLength);
                // If user provided covenant name, create it
                if (!newCovenant.isEmpty())
                {
                    // Create new covenant
                    if (DB_INSERT("Covenants", QString("NULL, '%1', '%2'").arg(newCovenant).arg(message.GetRealName())))
                    {
                        // Update covenant field for user
                        userData.UpdateUserData(message.GetRealName(), UDP_Covenant, newCovenant);
                        // Take price to create covenant
                        userData.UpdateUserData(message.GetRealName(), UDP_Currency,
                                                                       QString::number(currency.toInt()-price.toInt()));
                        // Set answer
                        answer = _answers.at(2);
                        answer.replace("COV_NAME", newCovenant);
                    }
                }
                // Name of covenant doesn't provided
                else
                {
                    answer = _answers.at(3);
                }
            }
            // Not enough currency
            else
            {
                answer = _answers.at(0);
            }
        }
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

QString CovenantCommand::_GetAnswerForCovRenameCmd(const ChatMessage& message)
{
    QString answer;
    UserData& userData = UserData::Instance();
    QString covenant = userData.GetUserDataParam(message.GetRealName(), UDP_Covenant);
    // Check if user in covenant
    if (covenant != "Viewer")
    {
        // Check if user is leader of its covenant
        std::shared_ptr<QSqlQuery> query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
        if (query->exec())
        {
            // If user is in covenant, check its leader
            if (query->first())
            {
                qDebug() << query->value("Leader").toString() << message.GetRealName();
                if (query->value("Leader").toString() == message.GetRealName())
                {
                    QString newCovenantName = message.GetMessage().mid(_name.size()+1);
                    int covNameLength = ((newCovenantName.size()-2) < 0) ? 0 : (newCovenantName.size()-2);
                    // If covenant name too long, just make it shorter
                    if (covNameLength > 50)
                    {
                        covNameLength = 50;
                    }
                    newCovenantName = newCovenantName.left(covNameLength);
                    // If user provided covenant name, create it
                    if (!newCovenantName.isEmpty())
                    {
                        // If covenant was disbanded, set covenant field to viewer for all users who was in that covenant
                        if (DB_UPDATE("Covenants", QString("Name = '%1'").arg(newCovenantName),
                                                   QString("Leader = '%1'").arg(message.GetRealName())))
                        {
                            std::shared_ptr<QSqlQuery> queryUpdate = DB_SELECT("UserData",
                                                                               "Name",
                                                                               QString("Covenant = '%1'").arg(covenant));
                            if (queryUpdate != NULL)
                            {
                                while (queryUpdate->next())
                                {
                                    userData.UpdateUserData(queryUpdate->value("Name").toString(), UDP_Covenant, newCovenantName);
                                }
                            }
                            answer = _answers.at(1);
                        }
                    }
                    else
                    {
                        answer = _answers.at(3);
                    }
                }
                else
                {
                    answer = _answers.at(0);
                }
            }
        }
    }
    else
    {
        answer = _answers.at(2);
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

int CovenantCommand::_GetEndOfNameFromAmpersand(int ampersandPosition, const QString& message)
{
    // Try to get name of user if user typed something after name of new leader
    int endOfName = message.indexOf(" ", ampersandPosition);
    // If user do not enter anything after name of a new leader
    if (endOfName < 0)
    {
        endOfName = message.indexOf("\r", ampersandPosition);
    }

    return endOfName;
}

///////////////////////////////////////////////////////////////////////////

bool CovenantCommand::_SetNewLeaderToCovenant(const QString& newLeader, const QString& oldLeader, const QString& covName)
{
    bool result(false);
    // Check if provided user is member of covenant
    std::shared_ptr<QSqlQuery> newLeaderQuery = DB_SELECT("UserData",
                                                          "Covenant",
                                                          QString("Name = '%1'").arg(newLeader));
    if (newLeaderQuery->exec())
    {
        if (newLeaderQuery->first())
        {
            // If so, make him leader
            if (newLeaderQuery->value("Covenant").toString() == covName)
            {
                // Change covenant of user
                UserData& userData = UserData::Instance();
                userData.UpdateUserData(oldLeader, UDP_Covenant, "Viewer");
                // Update leader name of covenant
                DB_UPDATE("Covenants", QString("Leader = '%1'").arg(newLeader),
                                       QString("Name = '%1'").arg(covName));
                result = true;
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
