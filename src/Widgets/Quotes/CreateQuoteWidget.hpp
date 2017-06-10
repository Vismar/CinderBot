/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QFrame>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>

namespace Ui
{

class CreateQuoteWidget : public QFrame
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit CreateQuoteWidget(QWidget* parent = 0);
    /*!
      * Desctructor
      */
    ~CreateQuoteWidget();

private slots:
    /*!
     * Adjust minimum possible size of chat message
     * \param(IN) size - New size of the document in message. In pixels.
     */
    void _AdjustMinimumSize(const QSizeF& size);
    /*!
     * Add quote that was typed in text field
     */
    void _AddQuote();

private:
    /*! Layout for widgets */
    QHBoxLayout* _layout;
    /*! Add button */
    QPushButton* _addButton;
    /*! Text field */
    QTextEdit*   _quoteText;
};

}
