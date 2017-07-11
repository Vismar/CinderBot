/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QTabWidget>
#include <QPointer>
#include "Widgets/Chat/ChatWindow.hpp"
#include "Widgets/CustomCommands/CustomCommandWindow.hpp"
#include "Widgets/Quotes/QuoteWindow.hpp"
#include "Widgets/Statistics/StatisticsWindow.hpp"
#include "Chat/TwitchClient.hpp"

namespace Ui
{

/*!
 * Class MainWindow
 * The MainWindow class contains all widgets and TwtichClient
 */
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /*** Creatign windows ***/
    /*!
     * Create and initialize chat window
     */
    void _CreateChatWindow();
    /*!
     * Create and inititalize custom command window
     */
    void _CreateCustomCommandWindow();
    /*!
     * Create and initialize quote window
     */
    void _CreateQuoteWindow();
    /*!
     * Create and initialize statistics window
     */
    void _CreateStatisticsWindow();

private:
    /*!
     * Initialize UI stuff.
     */
    void _InitUi();

    /*! Horizontal layout */
    QHBoxLayout *_layout;
    /*** Windows **/
    /*! Chat window */
    QPointer<ChatWindow> _chatWindow;
    /*! Custom command window */
    QPointer<CustomCommandWindow> _ccListWindow;
    /*! Quote window */
    QPointer<QuoteWindow> _quoteWindow;
    /*! Statistics window */
    QPointer<StatisticsWindow> _statisticsWindow;

    /*! Twitch client. Handles all network stuff. */
    TwitchClient *_twitchClient;
};

}
