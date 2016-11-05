#include "MainWindow.hpp"

using namespace Ui;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    InitUi();

    _chat = new ChatWidget(this);
    _chat->setFixedSize(300, 400);

    _twitchClient = new TwitchClient(this);
    connect(_twitchClient, &TwitchClient::NewMessage,
            _chat, &ChatWidget::AddEntry);
}

MainWindow::~MainWindow()
{
}

void MainWindow::InitUi()
{
    this->setWindowTitle("Cinder Bot");
    this->setFixedSize(640, 400);
}
