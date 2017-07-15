/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MainWindow.hpp"

using namespace Ui;
using namespace Ui::Login;
using namespace Ui::Chat;
using namespace Ui::CustomCommand;
using namespace Ui::Quote;
using namespace Ui::Analytics;

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) : QWidget(parent, Qt::Window)
{
    // Initialize twitch client
    _twitchClient = new TwitchClient(this);

    // Initialize UI
    _InitUi();
}

///////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
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

void MainWindow::_CreateAnalyticsWindow()
{
    // If window is closed, then create, connect and initialize
    if (_analyticsWindow.isNull())
    {
        _analyticsWindow = new StatisticsWindow(this);
        _analyticsWindow->setAttribute(Qt::WA_DeleteOnClose);
        _analyticsWindow->show();
    }
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_InitUi()
{
    // Title of app
    this->setWindowTitle("Cinder Bot");
    this->setWindowIcon(QIcon(":/Resources/Icons/IconEmber.ico"));
    // Fixed size
    int width = 64*4 + 5*5; // 4 columns, 5 - margin
    int height = 64*3 + 5*4; // 3 rows
    this->setFixedSize(width, height);

    // Layout
    _layout = new QGridLayout(this);
    _layout->setMargin(5);

    // Connect widget
    _ConnectWidget();

    // Buttons
    _ChatButton();
    _AnalyticsButton();
    _CustomCommandsButton();
    _QuotesButton();
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_ConnectWidget()
{
    _connectWidget = new ConnectWidget(this);
    _layout->addWidget(_connectWidget, 0, 0, 1, -1);
    connect(_connectWidget, &ConnectWidget::LoginSuccess,
            _twitchClient, &TwitchClient::Connect);
    connect(_twitchClient, &TwitchClient::ConnectionStateChanged,
            _connectWidget, &ConnectWidget::ChangeConnectionState);
    _connectWidget->CheckAndRunAutoLogin();
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_ChatButton()
{
    // Inititalize
    _chatButton = new QPushButton(this);
    _chatButton->setToolTip("Chat");
    _chatButton->setIcon(QIcon(":/Resources/Icons/ChatIcon.ico"));
    _chatButton->setIconSize(QSize(64,64));
    _chatButton->setFixedSize(65,65);
    connect(_chatButton, &QPushButton::clicked,
            this, &MainWindow::_CreateChatWindow);
    // Add to layout
    _layout->addWidget(_chatButton, 1, 0, Qt::AlignCenter);
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_AnalyticsButton()
{
    // Inititalize
    _analyticsButton = new QPushButton(this);
    _analyticsButton->setToolTip("Analytics");
    _analyticsButton->setIcon(QIcon(":/Resources/Icons/StatisticsIcon.ico"));
    _analyticsButton->setIconSize(QSize(64,64));
    _analyticsButton->setFixedSize(65,65);
    connect(_analyticsButton, &QPushButton::clicked,
            this, &MainWindow::_CreateAnalyticsWindow);
    // Add to layout
    _layout->addWidget(_analyticsButton, 1, 1, Qt::AlignCenter);
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_CustomCommandsButton()
{
    // Inititalize
    _customCommandButton = new QPushButton(this);
    _customCommandButton->setToolTip("Custom commands");
    _customCommandButton->setIcon(QIcon(":/Resources/Icons/CustomCommandsIcon.ico"));
    _customCommandButton->setIconSize(QSize(64,64));
    _customCommandButton->setFixedSize(65,65);
    connect(_customCommandButton, &QPushButton::clicked,
            this, &MainWindow::_CreateCustomCommandWindow);
    // Add to layout
    _layout->addWidget(_customCommandButton, 2, 0, Qt::AlignCenter);
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_QuotesButton()
{
    // Inititalize
    _quoteButton = new QPushButton(this);
    _quoteButton->setToolTip("Quotes");
    _quoteButton->setIcon(QIcon(":/Resources/Icons/QuotesIcon.ico"));
    _quoteButton->setIconSize(QSize(64,64));
    _quoteButton->setFixedSize(65,65);
    connect(_quoteButton, &QPushButton::clicked,
            this, &MainWindow::_CreateQuoteWindow);
    // Add to layout
    _layout->addWidget(_quoteButton, 2, 1, Qt::AlignCenter);
}

///////////////////////////////////////////////////////////////////////////
