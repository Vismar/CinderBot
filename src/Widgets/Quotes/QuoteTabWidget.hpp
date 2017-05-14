#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include "CreateQuoteWidget.hpp"
#include "QuotesWidget.hpp"

namespace Ui
{

class QuoteTabWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit QuoteTabWidget(QWidget* parent = 0);
    /*!
     * Destructor
     */
    ~QuoteTabWidget();

private:
    /*! Widget layout */
    QVBoxLayout*       _layout;
    /*! CreateQuote widget which helps to create quotes from bot app itself */
    CreateQuoteWidget* _createQuoteWidget;
    /*! Quotes widgets that contains all quotes */
    QuotesWidget*      _quotesWidget;
};

}
