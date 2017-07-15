/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QFrame>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>

namespace Ui
{
namespace Quote
{

class SingleQuoteWidget : public QFrame
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit SingleQuoteWidget(QWidget* parent = 0);
    /*!
     * Destructor
     */
    ~SingleQuoteWidget();

    /*!
     * Set number field for quote
     * \param number - number that should be setted
     */
    void SetQuoteNumber(const QString &number);
    /*!
     * Set text field for quote
     * \param text - text which will be used in text field
     */
    void SetQuoteText(const QString &text);

private slots:
    /*!
     * Adjust minimum possible size of chat message
     * \param(IN) size - New size of the document in message. In pixels.
     */
    void _AdjustMinimumSize(const QSizeF &size);
    /*!
     * Handle edit/save button
     */
    void _HandleEditSaveButton();
    /*!
     * Handle delete button
     */
    void _HandleDeleteButton();

private:
    /*! Widget layout */
    QHBoxLayout* _layout;
    /*! Quote number field */
    QLabel*      _quoteNumber;
    /*! Quote text field */
    QTextEdit*   _quoteText;
    /*! Quote edit/save button */
    QPushButton* _quoteEditSave;
    /*! Quote delete button */
    QPushButton* _quoteDelete;
};

}
}
