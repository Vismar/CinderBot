/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "CreateQuoteWidget.hpp"
#include "QuotesWidget.hpp"

/*!
 * \brief All Ui widgets and windows.
 */
namespace Ui
{
/*!
 * \brief All quote related widgets.
 */
namespace Quote
{

/*!
 * \brief Window that stores and displays list of quotes, also allows to create, edit and delete quotes.
 */
class QuoteWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QuoteWindow(QWidget* parent = nullptr);
    ~QuoteWindow();

public slots:
    /*!
     * \brief Adds new quote to list.
     * \param id - Id of quote that should be added.
     */
    void AddQuote(int id);
    /*!
     * \brief Deletes quotes from list.
     * \param id - Id of quote that should be deleted from list.
     */
    void DeleteQuote(int id);
    /*!
     * \brief Updates all quotes in list.
     * \param quoteNumber - unused param.
     */
    void UpdateQuotes(int quoteNumber);

private:
    /*! Widget layout */
    QVBoxLayout*       _layout;
    /*! CreateQuote widget which helps to create quotes from bot app itself */
    CreateQuoteWidget* _createQuoteWidget;
    /*! Quotes widgets that contains all quotes */
    QuotesWidget*      _quotesWidget;
};

}
}
