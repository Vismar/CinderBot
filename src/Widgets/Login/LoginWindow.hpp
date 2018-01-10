/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QWebEngineView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "Widgets/Common/EnhLineEdit.hpp"

/*!
 * \brief All Ui widgets and windows.
 */
namespace Ui
{
/*!
 * \brief Login widgets and windows.
 */
namespace Login
{

/*!
 * \brief Log-in window that allows you to configure the connection.
 *
 * Log-in window includes next field:
 *    - <b>Log-in.</b> Log-in which will be used by bot to send messages and other stuff.
 *    - <b>Channel.</b> Channel to which bot should connect.
 *    - <b>Auto log-in.</b> Small check box which defines if bot should try to auto log-in on start.
 * Pressing "Log In" will start process of checking entered values (if such channels exist) and
 * then opens small web view where user should "Allow" bot to do some things.
 * After that if everything went right, this window will be closed and connection process will be started.
 */
class LoginWindow : public QDialog
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget* parent = 0);
    ~LoginWindow();

signals:
    /*!
     * \brief Notify that log-in process was successful.
     *
     * This signal will be emitted only when token was received from twitch.tv.
     */
    void LoginSuccess();

private slots:
    /*!
     * \brief Checks login existence.
     *
     * Sends request to twitch to check if entered channel exists.
     */
    void _CheckLogin();
    /*!
     * \brief Checks room existence.
     *
     * Sends request to twitch to check if entered channel exists.
     */
    void _CheckRoom();
    /*!
     * \brief Checks what reply was received and decides what to do next.
     * \param reply - replay to parse.
     *
     * When network manager receives new reply, this function checks what kind of request it is
     * and then, depending on it, decides to what to do next. If request about login was successful,
     * launches check for room field. If request about room was successful, opens web view to start
     * authorization process.
     */
    void _CheckReply(QNetworkReply* reply);
    /*!
     * \brief Resets log-in error.
     * \param text - not used, but contains what text now contains in log-in field.
     *
     * Returns log-in field style-sheet to its default value.
     */
    void _ResetLoginError(const QString &text);
    /*!
     * \brief Resets room error.
     * \param text - not used, but contains what text now contains in channel field.
     *
     * Returns channel field style-sheet to its default value.
     */
    void _ResetRoomError(const QString &text);
    /*!
     * \brief Checks if url correct and contain token.
     * \param url - url to check.
     *
     * Checks if url contains "localhost" address and token field. If so, saves it,
     * emits signal about successful log-in process and closes dialog window.
     */
    void _CheckAndSaveToken(const QUrl &url);

private:
    /*!
     * \brief Initializes ui layout.
     *
     * Creates all widgets the are required and sets it to layout;
     */
    void _InitializeLayout();
    /*!
     * \brief Initializes error section.
     *
     * Initializes error label, sets its font size, alignment and etc.
     */
    void _InitErrorSection();
    /*!
     * \brief Initializes login section.
     *
     * Initializes login section that contains label and text field.
     */
    void _InitLoginSection();
    /*!
     * \brief Initializes room section.
     *
     * Initializes room section that contains label and text field.
     */
    void _InitRoomSection();
    /*!
     * \brief Initializes auto login section.
     *
     * Initializes auto login section which represented as check box.
     */
    void _InitAutoLoginSection();
    /*!
     * \brief Initializes button section.
     *
     *  Initializes authorization button.
     */
    void _InitButtonSection();
    /*!
     * \brief Initializes auth section.
     *
     * Initializes auth section that contains label which notify user about current auth step.
     */
    void _InitAuthSection();
    /*!
     * \brief Initializes web view section.
     *
     * Initializes web view to display authorization.
     */
    void _InitWebViewSection();

    /*!
     * \brief Turns on/off widgets in header.
     * \param active - flag to define active state of widgets.
     *
     * Turns on or off widgets in header to prevent or allow changes in fields and button click.
     */
    void _SetHeaderWidgetsActive(bool active);
    /*!
     * \brief Removes current error message.
     *
     * Hides current error message. Resizes window back to normal size.
     */
    void _DismissError();
    /*!
     * \brief Shows error message at the top of the window.
     *
     * Shows error message at the top of the window, resizes window to fit error message.
     */
    void _ShowError();
    /*!
     * \brief Loads web view to start authorization process.
     *
     * Resizes window to fir web view and opens web page to authorize bot to do stuff in channel.
     */
    void _LoadWebView();
    /*!
     * \brief Close window and emit success event.
     *
     * Save auto login param, emit success event and close dialog window.
     */
    void _CloseSuccess();

    /*! Network manager to check if entered channels exist. */
    QNetworkAccessManager *_netManager;

    /*! Simple layout to store widgets. */
    QGridLayout *_layout;
    /*! Label which displays errors. */
    QLabel *_errorLabel;
    /*! Log-in label. */
    QLabel *_loginLabel;
    /*! Log-in field which stores log-in that will be used by bot. */
    Common::EnhLineEdit *_login;
    /*! Channel label. */
    QLabel *_roomLabel;
    /*! Room field which stores channel to which bot should connect. */
    Common::EnhLineEdit *_room;
    /*! Check bot to try auto log-in process on the next start of bot, */
    QCheckBox *_autoLogin;
    /*! Push button start log-in process. */
    QPushButton *_loginButton;
    /*! Authorization label displays information about current authorization step. */
    QLabel *_authInfo;
    /*! WebView widget to authorization process to get the token. */
    QWebEngineView *_webView;

    /*! Small variable to store what step which was made most recently. */
    enum
    {
        NoRequest,
        LoginRequest,
        RoomRequest,
        OAuthRequest
    } _lastRequestType;

    /*! Small enum to store currently requested token. */
    enum
    {
        LoginToken,
        ChannelToken
    } _currentToken;
};

}
}
