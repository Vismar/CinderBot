/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MainWindow.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) : QWidget(parent, Qt::Window)
{
    // Initialize UI
    _InitUi();

    // Initialize twitch client
    _twitchClient = new TwitchClient(this);
}

///////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_InitUi()
{
    // Title of app
    this->setWindowTitle("Cinder Bot");
    this->setWindowIcon(QIcon(":/Resources/Icons/IconEmber.ico"));
    // Fixed size
    this->setFixedSize(300, 150);

    /*** Setting up widgets ***/
    _layout = new QGridLayout(this);

    // ChatButton setup
    _chatButton = new QPushButton(this);
    _chatButton->setToolTip("Chat");
    _chatButton->setIcon(QIcon(":/Resources/Icons/ChatIcon.ico"));
    _chatButton->setIconSize(QSize(64,64));
    _chatButton->setFixedSize(65,65);
    connect(_chatButton, &QPushButton::clicked,
            this, &MainWindow::_CreateChatWindow);

    // AnalyticsButton setup
    _analyticsButton = new QPushButton(this);
    _analyticsButton->setToolTip("Analytics");
    _analyticsButton->setIcon(QIcon(":/Resources/Icons/StatisticsIcon.ico"));
    _analyticsButton->setIconSize(QSize(64,64));
    _analyticsButton->setFixedSize(65,65);
    connect(_analyticsButton, &QPushButton::clicked,
            this, &MainWindow::_CreateStatisticsWindow);

    // CustomCommandButton setup
    _customCommandButton = new QPushButton(this);
    _customCommandButton->setToolTip("Custom commands");
    _customCommandButton->setIcon(QIcon(":/Resources/Icons/CustomCommandsIcon.ico"));
    _customCommandButton->setIconSize(QSize(64,64));
    _customCommandButton->setFixedSize(65,65);
    connect(_customCommandButton, &QPushButton::clicked,
            this, &MainWindow::_CreateCustomCommandWindow);

    // QuotesButton setup
    _quoteButton = new QPushButton(this);
    _quoteButton->setToolTip("Quotes");
    _quoteButton->setIcon(QIcon(":/Resources/Icons/QuotesIcon.ico"));
    _quoteButton->setIconSize(QSize(64,64));
    _quoteButton->setFixedSize(65,65);
    connect(_quoteButton, &QPushButton::clicked,
            this, &MainWindow::_CreateQuoteWindow);

    // Add widgets to layout
    _layout->addWidget(_chatButton, 0, 0);
    _layout->addWidget(_analyticsButton, 0, 1);
    _layout->addWidget(_customCommandButton, 1, 0);
    _layout->addWidget(_quoteButton, 1, 1);
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_CreateChatWindow()
{
    // If window is closed, then create, connect and initialize
    if (_chatWindow.isNull())
    {
        _chatWindow = new ChatWindow(this);
        connect(_twitchClient, &TwitchClient::NewMessage,
                _chatWindow.data(), &ChatWindow::AddEntryToChat);
        _chatWindow->setAttribute(Qt::WA_DeleteOnClose);
        _chatWindow->show();
    }
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_CreateCustomCommandWindow()
{
    // If window is closed, then create, connect and initialize
    if (_ccListWindow.isNull())
    {
        _ccListWindow = new CustomCommandWindow(this);
        _ccListWindow->setAttribute(Qt::WA_DeleteOnClose);
        _ccListWindow->show();
    }
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_CreateQuoteWindow()
{
    // If window is closed, then create, connect and initialize
    if (_quoteWindow.isNull())
    {
        _quoteWindow = new QuoteWindow(this);
        _quoteWindow->setAttribute(Qt::WA_DeleteOnClose);
        _quoteWindow->show();
    }
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_CreateStatisticsWindow()
{
    // If window is closed, then create, connect and initialize
    if (_statisticsWindow.isNull())
    {
        _statisticsWindow = new StatisticsWindow(this);
        _statisticsWindow->setAttribute(Qt::WA_DeleteOnClose);
        _statisticsWindow->show();
    }
}

///////////////////////////////////////////////////////////////////////////
