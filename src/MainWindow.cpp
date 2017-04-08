#include "MainWindow.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Initialize UI
    InitUi();

    // Create chat widget
    _chat = new ChatWidget(this);
    // Set chat widget size
    _chat->setFixedWidth(300);
    _chat->setMinimumHeight(370);
    // Add widget to layout
    _layout->addWidget(_chat);

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
    // Setting up layout
    _layout = new QHBoxLayout(this);
    _layout->setAlignment(Qt::AlignLeft);
    QWidget* window = new QWidget();
    window->setLayout(_layout);
    this->setCentralWidget(window);
    // Set minimum size for main window
    this->setMinimumSize(640, 400);
}

///////////////////////////////////////////////////////////////////////////
