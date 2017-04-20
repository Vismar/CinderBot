#include "RealTimeUserData.hpp"
#include <QStringList>

///////////////////////////////////////////////////////////////////////////

RealTimeUserData::RealTimeUserData(QObject* parent) : QObject(parent)
{
    _maxUserNumber = 0;
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

int RealTimeUserData::GetMaxUserNumber()
{
    return _maxUserNumber;
}

///////////////////////////////////////////////////////////////////////////

void RealTimeUserData::AddUserToList(const ChatMessage& chatMessage)
{
    if (!_userList.contains(chatMessage.GetAuthor()))
    {
        // Add user to list
        _userList.append(chatMessage.GetAuthor());
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
    _userList.removeOne(chatMessage.GetAuthor());
    emit UserListChanged();
}

///////////////////////////////////////////////////////////////////////////