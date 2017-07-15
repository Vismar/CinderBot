/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "LoginWindow.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

using namespace Ui::Login;

#define WINDOW_HEIGHT_NORMAL 125
#define WINDOW_HEIGHT_ERROR  150
#define WINDOW_HEIGHT_OATH   600

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
            _CheckRoom();
        }
        else if ( _lastRequestType == RoomRequest)
        {
            _LoadWebView();
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
            ConfigurationManager::Instance().SetStringParam(CFGP_LOGIN_OATH_KEY, token);

            // Notify about cussess athorization process
            emit LoginSuccess();

            // Close dialog window
            this->close();
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
    QString temp;
    // Default value
    _lastRequestType = NoRequest;
    // Init layout
    _layout = new QGridLayout(this);

    // State section
    _stateLabel = new QLabel(this);
    _stateLabel->setFixedSize(this->width() - 150, 22);
    _stateLabel->setAlignment(Qt::AlignCenter);

    _stateLabel->setStyleSheet("QLabel {"
                               "font: 11pt;"
                               "border: 1px solid black;"
                               "background-color : #FFFFFF;"
                               "color : #C00101; }");
    _layout->addWidget(_stateLabel, 0, 0, 1, 2, Qt::AlignCenter);
    _stateLabel->hide();

    // Login section
    _loginLabel = new QLabel(this);
    _loginLabel->setText("Login:");
    _layout->addWidget(_loginLabel, 1, 0, Qt::AlignRight);

    _login = new QLineEdit(this);
    _login->setPlaceholderText("Your bot account login");
    ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_NAME, temp);
    _login->setText(temp);
    _layout->addWidget(_login, 1, 1);
    connect(_login, &QLineEdit::textChanged,
            this, &LoginWindow::_ResetLoginError);

    // Room section
    _roomLabel = new QLabel(this);
    _roomLabel->setText("Channel:");
    _layout->addWidget(_roomLabel, 2, 0);

    _room = new QLineEdit(this);
    _room->setPlaceholderText("Channel to which bot should connect");
    ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_CHANNEL, temp);
    _room->setText(temp);
    _layout->addWidget(_room, 2, 1);
    connect(_room, &QLineEdit::textChanged,
            this, &LoginWindow::_ResetRoomError);

    // Auto login section
    _autoLogin = new QCheckBox(this);
    ConfigurationManager::Instance().GetStringParam(CFGP_LOGIN_AUTO, temp);
    if (temp == "true")
    {
        _autoLogin->setChecked(true);
    }
    _autoLogin->setText("Enable auto login");
    _layout->addWidget(_autoLogin, 3, 0, 1, 2, Qt::AlignCenter);

    // Button section
    _loginButton = new QPushButton(this);
    _loginButton->setFixedSize(200, 30);
    _loginButton->setText("Log In");
    _layout->addWidget(_loginButton, 4, 0, 1, 2, Qt::AlignCenter);
    connect(_loginButton, &QPushButton::clicked,
            this, &LoginWindow::_CheckLogin);

    // OAth section
    _webView = new QWebEngineView(this);
    _layout->addWidget(_webView, 5, 0, 1, 2, Qt::AlignCenter);
    connect(_webView, &QWebEngineView::urlChanged,
            this, &LoginWindow::_CheckAndSaveToken);
    _webView->hide();
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_SetHeaderWidgetsActive(bool active)
{
    // We need to use opposite value to proper call this functions
    _login->setReadOnly(!active);
    _room->setReadOnly(!active);
    _loginButton->setDisabled(!active);
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_DismissError()
{
    _login->setStyleSheet("");
    _room->setStyleSheet("");
    _stateLabel->hide();
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
        _stateLabel->setText("Error in login name");
        _stateLabel->show();
        break;
    case RoomRequest:
        _room->setStyleSheet("border: 1px solid red");
        _stateLabel->setText("Error in room name");
        _stateLabel->show();
        break;
    case OAthRequest:
        _stateLabel->setText("Error while authorization");
        _stateLabel->show();
        _webView->hide();
        break;
    default:
        break;
    }

    _lastRequestType = NoRequest;
}

///////////////////////////////////////////////////////////////////////////

void LoginWindow::_LoadWebView()
{
    // Save entered params
    ConfigurationManager &cfgMng = ConfigurationManager::Instance();
    cfgMng.SetStringParam(CFGP_LOGIN_NAME, _login->text());
    cfgMng.SetStringParam(CFGP_LOGIN_CHANNEL, _room->text());
    QString autoLogin = (_autoLogin->checkState() == Qt::Checked) ? "true" : "false";
    ConfigurationManager::Instance().SetStringParam(CFGP_LOGIN_AUTO, autoLogin);

    // Load url
    _lastRequestType = OAthRequest;
    this->setFixedHeight(WINDOW_HEIGHT_OATH);
    _webView->load(QUrl("https://api.twitch.tv/kraken/oauth2/authorize"
                        "?client_id=we0qz5ot41crhkeo1w6mv9t769x1q5"
                        "&redirect_uri=http://localhost&response_type=token"
                        "&scope=channel_check_subscription channel_commercial "
                        "channel_editor channel_feed_edit channel_subscriptions chat_login"));
    _webView->show();
}

///////////////////////////////////////////////////////////////////////////
