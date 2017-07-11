/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ChatWindow.hpp"
using namespace Ui;

///////////////////////////////////////////////////////////////////////////

ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent, Qt::Window)
{
    // Inititalize title and size
    this->setWindowTitle("Chat");
    this->setMinimumSize(300, 370);

    // Initialize widget and layout
    _layout = new QVBoxLayout(this);
    _layout->setMargin(0);
    _chat = new ChatWidget(this);
    _layout->addWidget(_chat);
}

///////////////////////////////////////////////////////////////////////////

ChatWindow::~ChatWindow() {}

///////////////////////////////////////////////////////////////////////////

void ChatWindow::AddEntryToChat(ChatMessage message, bool botMessage)
{
    _chat->AddEntry(message, botMessage);
}

///////////////////////////////////////////////////////////////////////////
