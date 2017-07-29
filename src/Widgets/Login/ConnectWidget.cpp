/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "ConnectWidget.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"

using namespace Ui::Login;
using namespace Utils::Configuration;

///////////////////////////////////////////////////////////////////////////

ConnectWidget::ConnectWidget(QWidget *parent) : QWidget(parent)
{
    // Set size
    this->setFixedWidth(parent->width());

    // Layout
    _layout = new QHBoxLayout(this);
    _layout->setMargin(0);
    _layout->setAlignment(Qt::AlignLeft);

    // Connect button
    _connectButton = new QPushButton(this);
    _connectButton->setText("Connect");
    _connectButton->setFixedHeight(64);
    _connectButton->setFixedWidth(this->width() - 64 - 10);
    connect(_connectButton, &QPushButton::clicked,
            this, &ConnectWidget::_OpenLoginWindow);

    // Connection state label
    _connectionStateLabel = new QLabel(this);
    _connectionStateLabel->setFixedSize(64, 64);
    ChangeConnectionState(NoConnection);

    // Add to layout
    _layout->addWidget(_connectButton);
    _layout->addWidget(_connectionStateLabel);
}

///////////////////////////////////////////////////////////////////////////

ConnectWidget::~ConnectWidget() {}

///////////////////////////////////////////////////////////////////////////

void ConnectWidget::CheckAndRunAutoLogin()
{
    QString paramLogin;
    QString paramOAuth;
    QString paramChannel;
    QString paramAuto;
    ConfigurationManager& cfgMng = ConfigurationManager::Instance();

    cfgMng.GetStringParam(LoginName, paramLogin);
    cfgMng.GetStringParam(LoginOauthKey, paramOAuth);
    cfgMng.GetStringParam(LoginChannel, paramChannel);

    // Wee ned to check that login, channel and oauth key exist in config file
    if (!paramLogin.isEmpty() && !paramChannel.isEmpty() && !paramOAuth.isEmpty())
    {
        cfgMng.GetStringParam(LoginAuto, paramAuto);
        if (paramAuto == "true")
        {
            emit LoginSuccess();
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void ConnectWidget::ChangeConnectionState(ConnectionState state)
{
    _connectionState = state;
    QPixmap pixmap;
    switch (_connectionState)
    {
    case NoConnection:
        pixmap = QPixmap(":/Resources/Icons/ConnectionStateNotConnected.ico").scaled(QSize(64,64), Qt::KeepAspectRatio);
        _connectionStateLabel->setToolTip("No connection");
        break;
    case Connecting:
        pixmap = QPixmap(":/Resources/Icons/ConnectionStateConnecting.ico").scaled(QSize(64,64), Qt::KeepAspectRatio);
        _connectionStateLabel->setToolTip("Connecting");
        break;
    case Connected:
        pixmap = QPixmap(":/Resources/Icons/ConnectionStateEstablished.ico").scaled(QSize(64,64), Qt::KeepAspectRatio);
        _connectionStateLabel->setToolTip("Connected");
        break;
    }
    _connectionStateLabel->setPixmap(pixmap);
}

///////////////////////////////////////////////////////////////////////////

void ConnectWidget::_OpenLoginWindow()
{
    if (_loginWindow.isNull())
    {
        _loginWindow = new LoginWindow();
        connect(_loginWindow.data(), &LoginWindow::LoginSuccess,
                this, &ConnectWidget::LoginSuccess);
        _loginWindow->setAttribute(Qt::WA_DeleteOnClose);
        _loginWindow->exec();
    }
}

///////////////////////////////////////////////////////////////////////////
