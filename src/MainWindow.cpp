#include "MainWindow.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Initialize UI
    InitUi();

    // Create chat widget
    _chat = new ChatWidget(this);
    // Set chat widget parameters
    // TODO: May be constant size should be changed to variable
    _chat->setFixedSize(300, 400);

    // Initialize twitch client
    _twitchClient = new TwitchClient(this);
    connect(_twitchClient, &TwitchClient::NewMessage,
            _chat, &ChatWidget::AddEntry);
}

///////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
}

///////////////////////////////////////////////////////////////////////////

void MainWindow::InitUi()
{
    // Title of app
    this->setWindowTitle("Cinder Bot");
    // Size of app
    // TODO: Should be changed to resizeable
    this->setFixedSize(640, 400);
}

///////////////////////////////////////////////////////////////////////////
