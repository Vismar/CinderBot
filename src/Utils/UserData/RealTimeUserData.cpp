#include "RealTimeUserData.hpp"
#include <QStringList>

///////////////////////////////////////////////////////////////////////////

RealTimeUserData::RealTimeUserData(QObject* parent) : QObject(parent) {}

///////////////////////////////////////////////////////////////////////////

RealTimeUserData* RealTimeUserData::Instance()
{
    static RealTimeUserData* realTimeUD = new RealTimeUserData();
    return realTimeUD;
}

///////////////////////////////////////////////////////////////////////////

RealTimeUserData::~RealTimeUserData() {}

///////////////////////////////////////////////////////////////////////////

const QStringList& RealTimeUserData::GetUserList()
{
    return _userList;
}

///////////////////////////////////////////////////////////////////////////

void RealTimeUserData::AddUserToList(const ChatMessage& chatMessage)
{
    if (!_userList.contains(chatMessage.GetAuthor()))
    {
        _userList.append(chatMessage.GetAuthor());
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
