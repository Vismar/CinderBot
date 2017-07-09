/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CommandList.hpp"

namespace Command
{
namespace UserDataCmd
{

/*!
 * Class UserDataCommandList
 * Store all UD commands
 */
class UserDataCommandList : public CommandList
{
public:
    /*! Constructor */
    UserDataCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void Initialize();
};

}
}
