/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QPushButton>
#include <QGridLayout>
#include <QPointer>
#include "Widgets/Chat/ChatWindow.hpp"
#include "Widgets/CustomCommands/CustomCommandWindow.hpp"
#include "Widgets/Quotes/QuoteWindow.hpp"
#include "Widgets/Statistics/StatisticsWindow.hpp"
#include "Widgets/Login/ConnectWidget.hpp"
#include "Chat/TwitchClient.hpp"

/*!
 * \brief All Ui widgets and windows.
 */
namespace Ui
{

/*!
 * \brief Main window which opens on launching bot app.
 *
 * The MainWindow class that contains buttons, each of them opens a specific window with certain functionality.
 * Also, initializes TwtichClient to proper work with twitch.tv.
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
     * \brief Create and initialize chat window.
     *
     * Public slot, which triggered by clicking on _chatButton.
     * Initializes window for chat, connects to twitch client to be updated, will be deleted automatically on close.
     */
    void _CreateChatWindow();
    /*!
     * \brief Create and inititalize custom command window
     *
     * Public slot, which triggered by clicking on _customCommandButton.
     * Initializes window for custom commands, will be deleted automatically on close.
     */
    void _CreateCustomCommandWindow();
    /*!
     * \brief Create and initialize quote window
     *
     * Public slot, which triggered by clicking on _quoteButton.
     * Initializes window for quotes, will be deleted automatically on close.
     */
    void _CreateQuoteWindow();
    /*!
     * \brief Create and initialize analytics window
     *
     * Public slot, which triggered by clicking on _analyticsButton.
     * Initializes window for analytics, will be deleted automatically on close.
     */
    void _CreateAnalyticsWindow();

private:
    /*!
     * \brief Initialize UI.
     *
     * Initialize title, icon and size of window, creating buttons in grid layout. Main initalize function.
     */
    void _InitUi();

    /*!
     * \brief Creates and initializes connect widget.
     *
     * Initializes connect widget which include connect button and connections state icon.
     */
    void _ConnectWidget();

    /*** App buttons ***/
    /*!
     * \brief Initialize and setup chat button.
     *
     * Creates chat buttons, setting up tooltip, icon, size and connecting with slot _CreateChatWindow.
     */
    void _ChatButton();
    /*!
     * \brief Initialize and setup analytics button.
     *
     * Creates analytics buttons, setting up tooltip, icon, size and connecting with slot _CreateAnalyticsWindow.
     */
    void _AnalyticsButton();
    /*!
     * \brief Initialize and setup custom commands button.
     *
     * Creates custom coomands buttons, setting up tooltip, icon, size and connecting with slot _CreateCustomCommandWindow.
     */
    void _CustomCommandsButton();
    /*!
     * \brief Initialize and setup quotes button.
     *
     * Creates quotes buttons, setting up tooltip, icon, size and connecting with slot _CreateQuoteWindow.
     */
    void _QuotesButton();

    /*** Widgets ***/
    /*! Horizontal layout */
    QGridLayout *_layout;
    /*! Connect widget */
    Login::ConnectWidget *_connectWidget;
    /*! Chat button which will open chat window */
    QPushButton *_chatButton;
    /*! Analyutics button which will open analytics window */
    QPushButton *_analyticsButton;
    /*! CustomCommand button which will open custom command window */
    QPushButton *_customCommandButton;
    /*! Quotes button which will open quotes window */
    QPushButton *_quoteButton;
    /*** Windows **/
    /*! Chat window */
    QPointer<Chat::ChatWindow> _chatWindow;
    /*! Custom command window */
    QPointer<CustomCommand::CustomCommandWindow> _ccListWindow;
    /*! Quote window */
    QPointer<Quote::QuoteWindow> _quoteWindow;
    /*! Statistics window */
    QPointer<Analytics::StatisticsWindow> _analyticsWindow;

    /*! Twitch client. Handles all network stuff. */
    TwitchClient *_twitchClient;
};

}
