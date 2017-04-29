#include "UserDataCommand.hpp"
#include "../../Utils/UserData/UserData.hpp"
#include "../../Utils/Config/ConfigurationManager.hpp"
#include "../../Utils/Config/ConfigurationParameters.hpp"

using namespace Command;

///////////////////////////////////////////////////////////////////////////

void UserDataCommand::SetCommandType(UDCommandType cmdType)
{
    _cmdType = cmdType;
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

QString UserDataCommand::GetRandomAnswer(const ChatMessage& message)
{
    QString answer;
    if (message.GetMessage().contains(_name))
    {
        UserData& userData = UserData::Instance();
        ConfigurationManager& configMng = ConfigurationManager::Instance();
        answer = _answers.first();
        answer.replace("MSG_COUNT", userData.GetUserDataParam(message.GetRealName() ,UDP_Messages));
        answer.replace("MSG_CUR", userData.GetUserDataParam(message.GetRealName() ,UDP_Currency));
        QString curName = "NomNom ";
        configMng.GetStringParam(CFGP_CURRENCY, curName);
        answer.replace("MSG_NAME_CUR", curName);
        answer.replace("MSG_COV", userData.GetUserDataParam(message.GetRealName() ,UDP_Covenant));
        _AddAuthorName(answer, message.GetAuthor());
    }

    return answer;
}

///////////////////////////////////////////////////////////////////////////

void UserDataCommand::Initialize()
{
    _Clear();
    switch (_cmdType)
    {
    case UDC_Messages:
        _name = "#UD_msg";
        //TODO: Replace with GetString in future
        _answers.push_back("You wrote MSG_COUNT messages! @");
        break;
    case UDC_Currency:
        _name = "#UD_cur";
        _answers.push_back("You have MSG_CUR of MSG_NAME_CUR ! @");
        break;
    case UDC_Covenant:
        _name = "#UD_cov";
        _answers.push_back("You are in MSG_COV covenant! @");
        break;
    case UDC_Full:
        _name = "#me";
        _answers.push_back("@: Message - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; Covenant - MSG_COV.");
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
