/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once

// Section names
#define CFGS_LOGIN     "LoginData"
#define CFGS_CONFIG    "ConfigData"
#define CFGS_IGNORE    "IgnoreList"
#define CFGS_USER      "User"
#define CFGS_COVENANT  "Covenant"

/*============================================*/
/*================ Parameters ================*/
/*============================================*/
/* Login params */
#define CFGP_LOGIN_NAME             "LoginName"
#define CFGP_LOGIN_OATH_KEY         "LoginOauthKey"
#define CFGP_LOGIN_CHANNEL          "LoginChannel"
/* Currency params */
#define CFGP_CURRENCY               "Currency"
#define CFGP_CURRENCY_PER_MSG       "CurrencyPerMsg"
#define CFGP_CURRENCY_OVER_TIME     "CurrencyOverTime"
#define CFGP_CURRECY_TIMER          "CurrencyTimer"
/* Covenant params */
#define CFGP_COV_JOIN_PRICE         "CovJoinPrice"
#define CFGP_COV_CREATE_PRICE       "CovCreatePrice"
#define CFGP_COV_RENAME_PRICE       "CovRenamePrice"
#define CFGP_VGRAPH_UPD_TIME        "ViewerGraphUpdateTime"
/* Message params */
#define CFGP_MESSAGE_GRAPH_UPD_TIME "MessagegraphUpdateTime"
