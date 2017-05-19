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
        _name = "#cov_list";
        _answers.push_back("Covenants: COV_LIST");
        break;
    case Cov_Join:
        _name = "#cov_join";
        _answers.push_back("You are joining COV_NAME, @.");
        _answers.push_back("You are already in that covenant! @");
        _answers.push_back("Not enough currency! @");
        break;
    case Cov_Leave:
        _name = "#cov_leave";
        _answers.push_back("You left covenant! Now you are just a viwer, @");
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

    return answer;
}

///////////////////////////////////////////////////////////////////////////

QString CovenantCommand::_GetAnswerForCovLeaveCmd(const ChatMessage& message)
{
    QString answer = _answers.first();
    UserData& userData = UserData::Instance();
    userData.UpdateUserData(message.GetRealName(), UDP_Covenant, "Viewer");

    return answer;
}

///////////////////////////////////////////////////////////////////////////
