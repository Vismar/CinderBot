/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Widgets/Chat/ChatWidget.hpp"

namespace Ui
{
namespace Chat
{

/*!
 * class ChatWindow
 * Simple chat window which display chat widget and other things
 */
class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit ChatWindow(QWidget *parent);
    /*!
     * Destructor
     */
    ~ChatWindow();

public slots:
    /*!
     * Adds message entry.
     * \param(IN) message
     * \param(IN) botMessage
     */
    void AddEntryToChat(ChatMessage message, bool botMessage);

private:
    /*! Layout for window */
    QVBoxLayout *_layout;

    /*** Widgets ***/
    /*! Chat widget */
    ChatWidget *_chat;
};

}
}
