/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Chat/ChatMessage.hpp"
#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>

namespace Ui
{

/*!
 * Class ChatWidget
 * Stores chat messages in vertical scrollable area.
 */
class ChatWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = 0);
    ~ChatWidget();

public slots:
    /*!
     * Adds message entry.
     * \param(IN) message
     * \param(IN) botMessage
     */
    void AddEntry(ChatMessage message, bool botMessage);

private slots:
    /*!
     * Scrolls area to the bottom.
     */
    void _AutoScrollDown();

private:
    /*! Vertical layout */
    QVBoxLayout* _layout;
    /*! Simple container to store scrol layout */
    QWidget*     _container;
};

}
