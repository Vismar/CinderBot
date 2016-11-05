#pragma once
#include <QMainWindow>
#include "./Widgets/ChatWidget.hpp"
#include "./Chat/TwitchClient.hpp"

namespace Ui
{

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    ChatWidget*   _chat;
    TwitchClient* _twitchClient;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitUi();
};

}
