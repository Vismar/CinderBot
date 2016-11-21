#include "UserDataChatCommand.hpp"
#include "../../Utils/UserData/UserData.hpp"
#include "../../Utils/Config/ConfigurationManager.hpp"
#include "../../Utils/Config/ConfigurationParameters.hpp"

#define UD_CURRNECY_DEFAULT "NomNom"
#define UD_COMMAND_MSG      "#UD_msg"
#define UD_COMMAND_MESSAGES "#messages"
#define UD_COMMAND_CUR      "#UD_cur"
#define UD_COMMAND_CURRENCY "#currency"
#define UD_COMMAND_COV      "#UD_cov"
#define UD_COMMAND_COVENANT "#covenant"
#define UD_COMMAND_FULL     "#me"

///////////////////////////////////////////////////////////////////////////

bool UserDataChatCommand::GetAnswer(ChatMessage& message, QString& answer)
{
    bool result(true);
    UserData& userData = UserData::Instance();
    // Handle message counter command
    if (message.GetMessage().contains(UD_COMMAND_MESSAGES) ||
        message.GetMessage().contains(UD_COMMAND_MSG))
    {
        answer = "Ты уже написал ";
        answer.append(userData.GetUserDataParam(message.GetAuthor(), UDP_Messages));
        answer.append(" сообщений!");
    }
    // Handle currency command
    else if (message.GetMessage().contains(UD_COMMAND_CURRENCY) ||
             message.GetMessage().contains(UD_COMMAND_CUR))
    {
        answer = "У тебя ";
        answer.append(userData.GetUserDataParam(message.GetAuthor(), UDP_Currency));
        answer.append(" ");
        QString param;
        ConfigurationManager& configMng = ConfigurationManager::Instance();
        // Get custom currency name
        if (configMng.GetStringParam(CFGS_CURRENCY, param))
        {
            answer.append(param);
        }
        //If it was not found, set default name
        else
        {
            answer.append(UD_CURRNECY_DEFAULT);
        }
        answer.append(" !");
    }
    // Handle covenant command
    else if (message.GetMessage().contains(UD_COMMAND_COVENANT) ||
             message.GetMessage().contains(UD_COMMAND_COV))
    {
        answer = "Ты состоишь в ковенанте \"";
        answer.append(userData.GetUserDataParam(message.GetAuthor(), UDP_Covenant));
        answer.append("\"!");
    }
    // Handle full statistics command
    else if (message.GetMessage().contains(UD_COMMAND_FULL))
    {
        answer = message.GetAuthor();
        answer.append(" - Твои данные: 1) Сообщений - ");
        answer.append(userData.GetUserDataParam(message.GetAuthor(), UDP_Messages));
        answer.append("; 2) ");
        QString param;
        ConfigurationManager& configMng = ConfigurationManager::Instance();
        if (configMng.GetStringParam(CFGS_CURRENCY, param))
        {
            answer.append(param);
        }
        else
        {
            answer.append(UD_CURRNECY_DEFAULT);
        }
        answer.append(" - ");
        answer.append(userData.GetUserDataParam(message.GetAuthor(), UDP_Currency));
        answer.append("; 3) Ковенант - ");
        answer.append(userData.GetUserDataParam(message.GetAuthor(), UDP_Covenant));
    }
    else
    {
        result = false;
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////
