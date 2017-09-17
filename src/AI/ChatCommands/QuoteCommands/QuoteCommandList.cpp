/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "QuoteCommandList.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "AI/ChatCommands/QuoteCommands/MainQuoteCommand.hpp"
#include "AI/ChatCommands/QuoteCommands/AddQuoteCommand.hpp"
#include "AI/ChatCommands/QuoteCommands/DeleteQuoteCommand.hpp"
#include "AI/ChatCommands/QuoteCommands/EditQuoteCommand.hpp"
#include "Utils/Database/DatabaseManager.hpp"

using namespace Command::QuoteCmd;
using namespace Utils::Configuration;
using namespace Utils::Database;

///////////////////////////////////////////////////////////////////////////

QuoteCommandList::QuoteCommandList()
{
    _commands.push_back(new MainQuoteCommand());
    _commands.push_back(new AddQuoteCommand());
    _commands.push_back(new DeleteQuoteCommand());
    _commands.push_back(new EditQuoteCommand());

    OnCfgParamChanged(CfgParam::QuotesCmdModule);
}

///////////////////////////////////////////////////////////////////////////

void QuoteCommandList::Initialize() { }

///////////////////////////////////////////////////////////////////////////

void QuoteCommandList::OnCfgParamChanged(CfgParam cfgParam)
{
    QString value;
    switch (cfgParam)
    {
    case CfgParam::QuotesCmdModule:
        ConfigurationManager::Instance().GetStringParam(CfgParam::QuotesCmdModule, value);
        _isTurnedOn = ("true" == value);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
