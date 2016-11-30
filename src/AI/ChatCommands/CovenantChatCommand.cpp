#include "CovenantChatCommand.hpp"
#include "../../Utils/UserData/UserData.hpp"
#include "../../Utils/Config/ConfigurationManager.hpp"
#include "../../Utils/Config/ConfigurationParameters.hpp"
#include <QStringList>

#define CCC_LIST  "#cov_list"
#define CCC_JOIN  "#cov_join"
#define CCC_LEAVE "#cov_leave"

///////////////////////////////////////////////////////////////////////////

bool CovenantChatCommand::GetAnswer(const ChatMessage& message, QString& answer)
{
    bool result(false);
    QString value;
    ConfigurationManager& configManager = ConfigurationManager::Instance();
    configManager.GetStringParam(CFGS_COVENANTS, value);
    // Display covenant list
    if (message.GetMessage().contains(CCC_LIST))
    {
        answer = "Covenants: ";
        answer.append(value);
        result = true;
    }
    // Try to join viewer to a specified covenant
    else if (message.GetMessage().contains(CCC_JOIN))
    {
        UserData& userData = UserData::Instance();
        int currency = userData.GetUserDataParam(message.GetAuthor(), UDP_Currency).toInt();
        // Set default price value
        int priceToJoin(100);
        // Try to get param from config manager
        QString price;
        if (configManager.GetStringParam(CFGP_COV_JOIN_PRICE,price))
        {
            priceToJoin = price.toInt();
        }
        // If user have enough currency to join
        if (currency >= priceToJoin)
        {
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
                        answer = "You are joining ";
                        answer.append(*iter+ ", @" + message.GetAuthor());
                        userData.UpdateUserData(message.GetAuthor(), UDP_Covenant, *iter);
                        userData.UpdateUserData(message.GetAuthor(), UDP_Currency, QString::number(currency - priceToJoin));
                    }
                    // User is already in that covenant
                    else
                    {
                        answer = "You are already in that covenant! @";
                        answer.append(message.GetAuthor());
                    }
                    result = true;
                    break;
                }
            }
        }
        // If user do not have enough currency
        else
        {
            answer = "Not enough currency! @";
            answer.append(message.GetAuthor());
            result = true;
        }
    }
    // User want to leave his covenant
    else if (message.GetMessage().contains(CCC_LEAVE))
    {
        UserData& userData = UserData::Instance();
        answer = "You left covenant! Now you are just a viwer, @";
        answer.append(message.GetAuthor());
        userData.UpdateUserData(message.GetAuthor(), UDP_Covenant, "Viewer");
        result = true;
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
