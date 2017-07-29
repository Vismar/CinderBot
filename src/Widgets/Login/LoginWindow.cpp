/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "LoginWindow.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include <QNetworkRequest>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

using namespace Ui::Login;
using namespace Ui::Common;
using namespace Utils::Configuration;

#define WINDOW_HEIGHT_NORMAL 125
#define WINDOW_HEIGHT_ERROR  150
#define WINDOW_HEIGHT_OAUTH  675

///////////////////////////////////////////////////////////////////////////

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent)
{
    // Init title and size
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowTitle("Login");
    this->setFixedSize(400, WINDOW_HEIGHT_NORMAL);

    // Init all content for window
    _InitializeLayout();

    _netManager = new QNetworkAccessManager(this);
    connect(_netManager, &QNetworkAccessManager::finished,
            this, &LoginWindow::_CheckReply);
}

///////////////////////////////////////////////////////////////////////////

LoginWindow::~LoginWindow() {}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_CheckLogin()
{
    if (_lastRequestType == NoRequest)
    {
        _DismissError();
        _SetHeaderWidgetsActive(false);
        _lastRequestType = LoginRequest;
        QString loginCheckRequest;
        loginCheckRequest.append("https://api.twitch.tv/kraken/users/");
        loginCheckRequest.append(_login->text());
        loginCheckRequest.append("?client_id=we0qz5ot41crhkeo1w6mv9t769x1q5");
        _netManager->get(QNetworkRequest(QUrl(loginCheckRequest)));
    }
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_CheckRoom()
{
    _lastRequestType = RoomRequest;
    QString roomCheckRequest;
    roomCheckRequest.append("https://api.twitch.tv/kraken/users/");
    roomCheckRequest.append(_room->text());
    roomCheckRequest.append("?client_id=we0qz5ot41crhkeo1w6mv9t769x1q5");
    _netManager->get(QNetworkRequest(QUrl(roomCheckRequest)));
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_CheckReply(QNetworkReply* reply)
{
    QString temp;
    QString replyString = reply->readAll();
    // If such user do not exist, show error
    if (replyString.startsWith("{\"error\":"))
    {
        _SetHeaderWidgetsActive(true);
        _ShowError();
    }
    // If user exist, check room or load athorization window
    else if (replyString.startsWith("{\"display_name\":"))
    {
        if (_lastRequestType == LoginRequest)
        {
            // Login param
            temp = _login->text();
            ConfigurationManager::Instance().SetStringParam(LoginName, temp.toLower());
            _CheckRoom();
        }
        else if ( _lastRequestType == RoomRequest)
        {
            // Channel param
            temp = _room->text();
            ConfigurationManager::Instance().SetStringParam(LoginChannel, temp.toLower());
            // Check if user already authorized
            QString loginOAuth;
            QString channelOAuth;
            ConfigurationManager::Instance().GetStringParam(LoginOauthKey, loginOAuth);
            ConfigurationManager::Instance().GetStringParam(LoginChannelOauthKey, channelOAuth);
            if (!loginOAuth.isEmpty() && !channelOAuth.isEmpty())
            {
                _CloseSuccess();
            }
            // If at least 1 oath key do not exist, start authorizatin process
            else
            {
                _LoadWebView();
            }
        }
    }
    // Wee MUST delete reply with delay
    reply->deleteLater();
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_ResetLoginError(const QString &text)
{
    Q_UNUSED(text);
    _login->setStyleSheet("");
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_ResetRoomError(const QString &text)
{
    Q_UNUSED(text);
    _room->setStyleSheet("");
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_CheckAndSaveToken(const QUrl &url)
{
    // Get url
    QString urlString = url.url();
    // Check if it was redirected to localhost
    if (urlString.startsWith("http://localhost/"))
    {
        // Try to find token
        QRegularExpression regExp("#access_token=(?<token>.*)&");
        QRegularExpressionMatch match = regExp.match(urlString);
        // If token was found, save it
        if (match.hasMatch())
        {
            QString token = match.captured("token");
            switch (_currentToken)
            {
            case LoginToken:
                // Set login oauth key in config
                ConfigurationManager::Instance().SetStringParam(LoginOauthKey, token);
                _currentToken = ChannelToken;
                // Hide loading second authorization
                _webView->hide();
                this->setFixedHeight(WINDOW_HEIGHT_NORMAL);
                _LoadWebView();
                break;
            case ChannelToken:
                // Set channel oauth key in config
                ConfigurationManager::Instance().SetStringParam(LoginChannelOauthKey, token);
                _CloseSuccess();
                break;
            }
        }
        else
        {
            _ShowError();
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_InitializeLayout()
{
    // Default value
    _lastRequestType = NoRequest;
    _currentToken = LoginToken;
    // Init layout
    _layout = new QGridLayout(this);

    // Inititalize widget sections
    _InitErrorSection();
    _InitLoginSection();
    _InitRoomSection();
    _InitAutoLoginSection();
    _InitButtonSection();
    _InitAuthSection();
    _InitWebViewSection();
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_InitErrorSection()
{
    _errorLabel = new QLabel(this);
    _errorLabel->setFixedSize(this->width() - 150, 22);
    _errorLabel->setAlignment(Qt::AlignCenter);

    _errorLabel->setStyleSheet("QLabel {"
                               "font: 11pt;"
                               "border: 1px solid black;"
                               "background-color : #FFFFFF;"
                               "color : #C00101; }");
    _layout->addWidget(_errorLabel, 0, 0, 1, 2, Qt::AlignCenter);
    _errorLabel->hide();
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_InitLoginSection()
{
    QString temp;
    // Label
    _loginLabel = new QLabel(this);
    _loginLabel->setText("Bot login:");
    _layout->addWidget(_loginLabel, 1, 0, Qt::AlignRight);
    // Text field
    _login = new EnhLineEdit(this);
    _login->setPlaceholderText("Your bot account login");
    ConfigurationManager::Instance().GetStringParam(LoginName, temp);
    _login->setText(temp);
    _layout->addWidget(_login, 1, 1);
    connect(_login, &EnhLineEdit::textChanged,
            this, &LoginWindow::_ResetLoginError);
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_InitRoomSection()
{
    QString temp;
    // Label
    _roomLabel = new QLabel(this);
    _roomLabel->setText("Channel:");
    _layout->addWidget(_roomLabel, 2, 0);
    // Text field
    _room = new EnhLineEdit(this);
    _room->setPlaceholderText("Channel to which bot should connect");
    ConfigurationManager::Instance().GetStringParam(LoginChannel, temp);
    _room->setText(temp);
    _layout->addWidget(_room, 2, 1);
    connect(_room, &EnhLineEdit::textChanged,
            this, &LoginWindow::_ResetRoomError);
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_InitAutoLoginSection()
{
    QString temp;
    _autoLogin = new QCheckBox(this);
    ConfigurationManager::Instance().GetStringParam(LoginAuto, temp);
    if (temp == "true")
    {
        _autoLogin->setChecked(true);
    }
    _autoLogin->setText("Enable auto login");
    _layout->addWidget(_autoLogin, 3, 0, 1, 2, Qt::AlignCenter);
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_InitButtonSection()
{
    _loginButton = new QPushButton(this);
    _loginButton->setFixedSize(200, 30);
    _loginButton->setText("Authorize");
    _layout->addWidget(_loginButton, 4, 0, 1, 2, Qt::AlignCenter);
    connect(_loginButton, &QPushButton::clicked,
            this, &LoginWindow::_CheckLogin);
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_InitAuthSection()
{
    _authInfo = new QLabel(this);
    _authInfo->setStyleSheet("QLabel {background-color=\"white\"}");
    _layout->addWidget(_authInfo, 5, 0, 1, 2, Qt::AlignCenter);
    _authInfo->hide();
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_InitWebViewSection()
{
    _webView = new QWebEngineView(this);
    _webView->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    _layout->addWidget(_webView, 6, 0, 1, 2, Qt::AlignCenter);
    connect(_webView, &QWebEngineView::urlChanged,
            this, &LoginWindow::_CheckAndSaveToken);
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_SetHeaderWidgetsActive(bool active)
{
    // We need to use opposite value to proper call this functions
    _login->SetEditable(active);
    _room->SetEditable(active);
    _loginButton->setDisabled(!active);
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_DismissError()
{
    _login->setStyleSheet("");
    _room->setStyleSheet("");
    _errorLabel->hide();
    this->setFixedHeight(WINDOW_HEIGHT_NORMAL);
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_ShowError()
{
    // Change size of window
    this->setFixedHeight(WINDOW_HEIGHT_ERROR);
    // Do that depends on state
    switch (_lastRequestType)
    {
    case LoginRequest:
        _login->setStyleSheet("border: 1px solid red");
        _errorLabel->setText("Error in login name");
        _errorLabel->show();
        break;
    case RoomRequest:
        _room->setStyleSheet("border: 1px solid red");
        _errorLabel->setText("Error in room name");
        _errorLabel->show();
        break;
    case OAuthRequest:
        _errorLabel->setText("Error while authorization");
        _errorLabel->show();
        _webView->hide();
        _authInfo->hide();
        break;
    default:
        break;
    }

    _lastRequestType = NoRequest;
    _currentToken = LoginToken;
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_LoadWebView()
{
    // Show web view
    this->setFixedHeight(WINDOW_HEIGHT_OAUTH);
    _webView->show();

    // Check state
    if (_currentToken == LoginToken)
    {
        // Load url
        _lastRequestType = OAuthRequest;
        _webView->load(QUrl("https://api.twitch.tv/kraken/oauth2/authorize"
                            "?client_id=we0qz5ot41crhkeo1w6mv9t769x1q5"
                            "&redirect_uri=http://localhost&response_type=token"
                            "&force_verify=true"
                            "&scope=channel_editor+channel_subscriptions+chat_login"));

        // Notify user about first authorization
        _authInfo->setText("<center><font size=\"5\" color=\"green\">"
                           "<b>First authorization</b> - Bot account</font></center>"
                           "<center><font size=\"4\" color=\"grey\">"
                           "<b>Second authorization</b> - Stream account</font></center>");
        _authInfo->show();
    }
    else
    {
        // Reset cookies to auto log out user before second authorization
        _webView->page()->profile()->cookieStore()->deleteAllCookies();
        // Load url for channel auth
        _webView->load(QUrl("https://api.twitch.tv/kraken/oauth2/authorize"
                            "?client_id=we0qz5ot41crhkeo1w6mv9t769x1q5"
                            "&redirect_uri=http://localhost&response_type=token"
                            "&force_verify=true"
                            "&scope=channel_commercial+channel_editor+"
                                   "channel_feed_edit+channel_subscriptions"));

        // Notify user about second authorization
        _authInfo->setText("<center><font size=\"4\" color=\"grey\">"
                           "<b>&#10004; First authorization</b> - Bot account</font></center>"
                           "<center><font size=\"5\" color=\"green\">"
                           "<b>Second authorization</b> - Stream account</font></center>");
        _authInfo->show();
    }
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_CloseSuccess()
{
    //Save auto login param
    QString autoLogin = (_autoLogin->checkState() == Qt::Checked) ? "true" : "false";
    ConfigurationManager::Instance().SetStringParam(LoginAuto, autoLogin);
    // Notify about cussess athorization process
    emit LoginSuccess();
    // Close dialog window
    this->close();
}

///////////////////////////////////////////////////////////////////////////
