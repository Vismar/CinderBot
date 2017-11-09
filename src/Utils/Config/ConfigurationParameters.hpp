/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once

/*!
 * \brief Contains all utility classes.
 */
namespace Utils
{

/*!
 * \brief Contains configuration classes.
 */
namespace Configuration
{

/*!
 * \brief All configuration parameters.
 */
enum class CfgParam
{
    UndefinedCfgParam = -1,
    // Login params
    LoginName = 0,
    LoginDisplayName,
    LoginNameColor,
    LoginOauthKey,
    LoginChannel,
    LoginChannelOauthKey,
    LoginAuto,
    // Currency params
    Currency,
    CurrencyPerMsg,
    CurrencyOverTime,
    CurrencyTimer,
    CurrencyPerBit,
    // Covenant params
    CovJoinPrice,
    CovCreatePrice,
    CovRenamePrice,
    CovExpToLvl,
    CovExpTimer,
    CovExpGain,
    CovMembersDefault,
    CovMembersPerUp,
    CovMembersLvlRequirement,
    CovCmdSlotsDefault,
    CovCmdSlotsPerUp,
    CovCmdSlotsLvlRequirement,
    // Analytics params
    ViewerGraphUpdateTime,
    MessageGraphUpdateTime,
    // Additional params
    IgnoreList,
    // Inbuilt command modules
    UserDataCmdModule,
    UserDataCmdRcvWhisper,
    UserDataCmdRcvChat,
    QuotesCmdModule,
    CovenantCmdModule,
    CovenantCmdRcvWhisper,
    CovenantCmdRcvChat,
    CustomCmdModule,
    EndOfParams
};

}
}
