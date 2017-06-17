/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include "Widgets/Chat/ChatWidget.hpp"
#include "Widgets/Statistics/StatisticsWidget.hpp"
#include "Widgets/Quotes/QuoteTabWidget.hpp"
#include "Chat/TwitchClient.hpp"

namespace Ui
{

/*!
 * Class MainWindow
 * The MainWindow class contains all widgets and TwtichClient
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    /*!
     * Initialize UI stuff.
     */
    void _InitUi();

    /*!
     * Create and initialize chat widget
     */
    void _CreateChatWidget();

    /*!
     * Create and initialize tab widget
     */
    void _CreateTabWidget();

    /*!
     * Create and initialize statistics widget
     */
    void _CreateStatisticsWidget();
    /*!
     * Create and initialize quotes widget
     */
    void _CreateQuotesWidget();

    /*! Horizontal layout */
    QHBoxLayout*      _layout;
    /*! Scrollable chat widget */
    ChatWidget*       _chat;
    /*! Tab widhet */
    QTabWidget*       _tabWidget;
    /*! Statistics widget */
    StatisticsWidget* _statisticsWidget;
    /*! Quotes widget */
    QuoteTabWidget*   _quoteTabWidget;
    /*! Twitch client. Handles all network stuff. */
    TwitchClient*     _twitchClient;
};

}
