#pragma once
#include <QMainWindow>
#include "./Widgets/ChatWidget.hpp"
#include "./Chat/TwitchClient.hpp"

namespace Ui
{

/*!
 * \class MainWindow
 *
 * The MainWindow class contains all widgets and TwtichClient
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    /*!
     * Scrollable chat widget
     */
    ChatWidget*   _chat;
    /*!
     * Twitch client. Handles all network stuff.
     */
    TwitchClient* _twitchClient;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*!
     * Initialize UI stuff.
     */
    void InitUi();
};

}
