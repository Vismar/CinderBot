/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QMainWindow>
#include <QVBoxLayout>
#include "CreateQuoteWidget.hpp"
#include "QuotesWidget.hpp"

namespace Ui
{

class QuoteWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit QuoteWindow(QWidget* parent = 0);
    /*!
     * Destructor
     */
    ~QuoteWindow();

public slots:
    /*!
     * Update quotes data in paage list widget
     * \param tableName - name of table
     */
    void UpdateQuotes(const QString &tableName);

private:
    /*! Widget layout */
    QVBoxLayout*       _layout;
    /*! CreateQuote widget which helps to create quotes from bot app itself */
    CreateQuoteWidget* _createQuoteWidget;
    /*! Quotes widgets that contains all quotes */
    QuotesWidget*      _quotesWidget;
};

}
