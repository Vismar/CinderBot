/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPointer>
#include "Widgets/Login/LoginWindow.hpp"
#include "Chat/TwitchClient.hpp"

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
 * \brief Widget which contains connect button and connection state
 *
 * The ConnectWidget class contains button(opens LoginDataWindow) and connection state.
 * State can be next:
 *    - <b>No connection.</b> Bot not connected to twitch.tv.
 *    - <b>Connecting.</b> Bot in process of connection to twitch.tv.
 *    - <b>Connected.</b> Bot connected to twitch.tv.
 */
class ConnectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectWidget(QWidget* parent = 0);
    ~ConnectWidget();

    /*!
     * \brief Checks possibility of auto login and does it if everything is ok.
     *
     * Checks if login, channel and oauth key exist in config manager
     * when checks if auto login param is "true".
     * If everything exists and param value is correct, auto-login process will be launched.
     */
    void CheckAndRunAutoLogin();

signals:
    /*!
     * \brief Signal about successful login process.
     *
     * This signal will be emitted only when bot will receive token from twitch api.
     */
    void LoginSuccess();

public slots:
    /*!
     * \brief Changes connection state, update icon and tooltip.
     * \param state - new state which should be used.
     *
     * Changes connection state and, depends on it, updates current icon and tooltip for it.
     */
    void ChangeConnectionState(ConnectionState state);

private slots:
    /*!
     * \brief Opens LoginWindow.
     *
     * Opens LoginWindow, where user can enter his Log-in name and channel name to which bot should connect.
     * Moreover, user can choose to auto log-in on app launch for next time.
     */
    void _OpenLoginWindow();

private:
    /*! Main layout */
    QHBoxLayout *_layout;
    /*! Connect button */
    QPushButton *_connectButton;
    /*! Connection state image */
    QLabel *_connectionStateLabel;
    /*! LoginWindow */
    QPointer<LoginWindow> _loginWindow;

    /*! Connection state. Depending on it icon and tooltip for it will be updated in slot ChangeConnectionState(). */
    ConnectionState _connectionState;
};

}
}
