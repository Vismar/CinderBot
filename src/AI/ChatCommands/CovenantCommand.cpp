#include "CovenantCommand.hpp"
#include "../../Utils/UserData/UserData.hpp"
#include "../../Utils/Config/ConfigurationManager.hpp"
#include "../../Utils/Config/ConfigurationParameters.hpp"
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
        QString value;
        QString price;
        ConfigurationManager& configMng = ConfigurationManager::Instance();
        UserData& userData = UserData::Instance();
        switch (_cmdType)
        {
        case Cov_List:
            answer = _answers.first();
            configMng.GetStringParam(CFGS_COVENANTS,value);
            answer.replace("COV_LIST", value);
            break;
        case Cov_Join:
            {
            int currency = userData.GetUserDataParam(message.GetAuthor(), UDP_Currency).toInt();
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
                configMng.GetStringParam(CFGS_COVENANTS,value);
                QStringList covenants = value.split(",");
                // Try to find specified covenant from list of covenants
                for (auto iter = covenants.begin(); iter != covenants.end(); ++iter)
                {
                    // Found a overlap
                    if (message.GetMessage().contains(*iter))
                    {
                        // Check if user already in this covenant
                        if (userData.GetUserDataParam(message.GetAuthor(), UDP_Covenant) != *iter)
                        {
                            // Join user to covenant and take currency for it
                            answer = _answers.first();
                            answer.replace("COV_NAME", *iter);
                            userData.UpdateUserData(message.GetAuthor(), UDP_Covenant, *iter);
                            userData.UpdateUserData(message.GetAuthor(), UDP_Currency, QString::number(currency - priceToJoin));
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
            break;
        case Cov_Leave:
            answer = _answers.first();
            userData.UpdateUserData(message.GetAuthor(), UDP_Covenant, "Viewer");
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
