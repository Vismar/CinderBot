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

    _CreateChatWindow();
    _CreateCustomCommandWindow();
    _CreateQuoteWindow();
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
    // Setting up layout
    _layout = new QHBoxLayout(this);
    _layout->setAlignment(Qt::AlignLeft);
    // Set minimum size for main window
    this->setMinimumSize(850, 605);

    // Create widgets
    _CreateTabWidget();
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

void MainWindow::_CreateTabWidget()
{
    // Initialize tab widget
    _tabWidget = new QTabWidget(this);
    _tabWidget->setTabPosition(QTabWidget::East);
    _tabWidget->setStyleSheet("QTabBar::tab {height: 43px; width:43px; padding:1px; margin:1px;}"
                              "QTabBar::tab:selected {height: 43px; width:43px; padding:1px; margin:1px; background: #383838;}");
    _tabWidget->setIconSize(QSize(40, 40));

    /***** First tab - Statistics *****/
    // Create and add statistics widget
    _CreateStatisticsWidget();
    QIcon iconStat(":/Resources/Icons/StatisticsIcon.ico");
    _tabWidget->addTab(_statisticsWidget, iconStat, "");
    _tabWidget->setTabToolTip(0, "Statistics");

    // Add tab widget to layout
    _layout->addWidget(_tabWidget);
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_CreateStatisticsWidget()
{
    _statisticsWidget = new StatisticsWidget();
}

///////////////////////////////////////////////////////////////////////////
