/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "RealTimeUserData.hpp"
#include <QStringList>

///////////////////////////////////////////////////////////////////////////

RealTimeUserData::RealTimeUserData(QObject* parent) : QObject(parent)
{
    _maxUserNumber = 0;
    _msgCounter = 0;
}

///////////////////////////////////////////////////////////////////////////

RealTimeUserData* RealTimeUserData::Instance()
{
    static RealTimeUserData realTimeUD;
    RealTimeUserData* realTimeUDPointer = &realTimeUD;
    return realTimeUDPointer;
}

///////////////////////////////////////////////////////////////////////////

RealTimeUserData::~RealTimeUserData() {}

///////////////////////////////////////////////////////////////////////////

const QStringList& RealTimeUserData::GetUserList()
{
    return _userList;
}

///////////////////////////////////////////////////////////////////////////

const QStringList& RealTimeUserData::GetModeList()
{
    return _modeList;
}

///////////////////////////////////////////////////////////////////////////

int RealTimeUserData::GetMaxUserNumber()
{
    return _maxUserNumber;
}

///////////////////////////////////////////////////////////////////////////

void RealTimeUserData::AddUserToList(const ChatMessage& chatMessage)
{
    if (!_userList.contains(chatMessage.GetRealName()))
    {
        // Add user to list
        _userList.append(chatMessage.GetRealName());
        // Check max number of users
        if (_userList.size() > _maxUserNumber)
        {
            _maxUserNumber = _userList.size();
        }
        emit UserListChanged();
    }
}

///////////////////////////////////////////////////////////////////////////

void RealTimeUserData::RemoveUserFromList(const ChatMessage& chatMessage)
{
    _userList.removeOne(chatMessage.GetRealName());
    emit UserListChanged();
}

///////////////////////////////////////////////////////////////////////////

void RealTimeUserData::AddModeToList(const ChatMessage& chatMessage)
{
    if (!_modeList.contains(chatMessage.GetRealName()))
    {
        _modeList.append(chatMessage.GetRealName());
        emit ModeListChanged();
    }
}

///////////////////////////////////////////////////////////////////////////

void RealTimeUserData::RemoveModeFromList(const ChatMessage& chatMessage)
{
    _modeList.removeOne(chatMessage.GetRealName());
    emit ModeListChanged();
}

///////////////////////////////////////////////////////////////////////////

void RealTimeUserData::IncrementMsgCounter()
{
    ++_msgCounter;
}

///////////////////////////////////////////////////////////////////////////

unsigned long long RealTimeUserData::GetMsgCounter() const
{
    return _msgCounter;
}

///////////////////////////////////////////////////////////////////////////
