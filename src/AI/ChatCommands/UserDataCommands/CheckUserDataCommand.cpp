/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "CheckUserDataCommand.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/DatabaseManager.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Command::UserDataCmd;
using namespace Utils::Configuration;

#define MSG_NOT_LEADER 0
#define MSG_NO_COV     1
#define MSG_IS_LEADER  2
#define MSG_NO_NAME    3
#define MSG_NO_USER    4

///////////////////////////////////////////////////////////////////////////

CheckUserDataCommand::CheckUserDataCommand()
{
    Initialize();
}

///////////////////////////////////////////////////////////////////////////

void CheckUserDataCommand::Initialize()
{
    _name = "!check";
    _moderatorOnly = true;
    _answers.push_back("USER_NAME: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Covenant - MSG_COV.");
    _answers.push_back("USER_NAME: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Not in covenant.");
    _answers.push_back("USER_NAME: Messages - MSG_COUNT; MSG_NAME_CUR - MSG_CUR; "
                       "Time spent in chat - MSG_TIME_IN_CHAT; Leader of covenant 'MSG_COV'.");
    _answers.push_back("Please provide a name, @!");
    _answers.push_back("User with such name is not exist, @!");
}

///////////////////////////////////////////////////////////////////////////

void CheckUserDataCommand::_GetAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    if (_CheckModerationFlag(message.IsModerator()))
    {
        QString userName;
        if (message.GetMessage().size() > _name.size())
        {
            userName = message.GetMessage().right(message.GetMessage().size() - _name.size() - 1);
        }
        if (!userName.isEmpty())
        {
            DB_QUERY_PTR userQuery = DB_SELECT("UserData", "*", QString("Author='%1'").arg(userName));
            if ((userQuery != nullptr) && (userQuery->first()))
            {
                QString covenant = UD_GET_PARAM(userQuery->value("Name").toString() ,UDP_Covenant);
                DB_QUERY_PTR query = DB_SELECT("Covenants", "Leader", QString("Name = '%1'").arg(covenant));
                if (query != nullptr)
                {
                    if (query->first())
                    {
                        if (query->value("Leader").toString() == userQuery->value("Name"))
                        {
                            answer.AddAnswer(_answers.at(MSG_IS_LEADER));
                        }
                    }
                }
                if (answer.GetAnswers().isEmpty())
                {
                    if (covenant == "Viewer")
                    {
                        answer.AddAnswer(_answers.at(MSG_NO_COV));
                    }
                    else
                    {
                        answer.AddAnswer(_answers.at(MSG_NOT_LEADER));
                    }
                }
                if (!answer.GetAnswers().isEmpty())
                {
                    QString realName = userQuery->value("Name").toString();
                    auto firstAnswer = answer.GetAnswers().begin();
                    QString curName = "NomNom ";
                    ConfigurationManager::Instance().GetStringParam(CfgParam::Currency, curName);
                    (*firstAnswer).replace("MSG_NAME_CUR", curName);
                    (*firstAnswer).replace("MSG_COUNT", UD_GET_PARAM(realName, UDP_Messages));
                    (*firstAnswer).replace("MSG_CUR", UD_GET_PARAM(realName, UDP_Currency));
                    (*firstAnswer).replace("MSG_COV", UD_GET_PARAM(realName, UDP_Covenant));
                    (*firstAnswer).replace("USER_NAME", userName);
                    // Time in chat
                    int minutes = UD_GET_PARAM(realName, UDP_TimeInChat).toInt();
                    int hours = minutes / 60;
                    minutes %= 60;
                    (*answer.GetAnswers().begin()).replace("MSG_TIME_IN_CHAT", QString("%1h%2m").arg(hours).arg(minutes));
                }
            }
            else
            {
                answer.AddAnswer(_answers.at(MSG_NO_USER));
            }
        }
        else
        {
            answer.AddAnswer(_answers.at(MSG_NO_NAME));
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void CheckUserDataCommand::_GetRandomAnswer(const ChatMessage &message, ChatAnswer &answer)
{
    Q_UNUSED(message);
    Q_UNUSED(answer);
}

///////////////////////////////////////////////////////////////////////////
