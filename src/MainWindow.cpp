/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "MainWindow.hpp"

#include "Widgets/CustomCommands/CustomCommandWindow.hpp"
#include "Widgets/Quotes/QuoteWindow.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) : QWidget(parent, Qt::Window)
{
    // Initialize UI
    _InitUi();

    // Initialize twitch client
    _twitchClient = new TwitchClient(this);
    connect(_twitchClient, &TwitchClient::NewMessage,
            _chat, &ChatWidget::AddEntry);
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
    _CreateChatWidget();
    _CreateTabWidget();
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_CreateChatWidget()
{
    // Create chat widget
    _chat = new ChatWidget(this);
    // Set chat widget size
    _chat->setFixedWidth(300);
    _chat->setMinimumHeight(370);
    // Add widget to layout
    _layout->addWidget(_chat);
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

    /***** SHOULD BE REMOVED WHEN NEW INTERFACE WILL BE IMPLEMENTED *****/
    CustomCommandWindow *ccListWindow = new CustomCommandWindow(this);
    ccListWindow->setAttribute(Qt::WA_DeleteOnClose);
    ccListWindow->show();

    QuoteWindow *quoteWindow = new QuoteWindow(this);
    quoteWindow->setAttribute(Qt::WA_DeleteOnClose);
    quoteWindow->show();
    /***** SHOULD BE REMOVED WHEN NEW INTERFACE WILL BE IMPLEMENTED *****/

    // Add tab widget to layout
    _layout->addWidget(_tabWidget);
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::_CreateStatisticsWidget()
{
    _statisticsWidget = new StatisticsWidget();
}

///////////////////////////////////////////////////////////////////////////
