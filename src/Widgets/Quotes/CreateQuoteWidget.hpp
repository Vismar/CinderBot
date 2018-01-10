/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QPushButton>
#include <QHBoxLayout>
#include "Widgets/Common/EnhTextEdit.hpp"

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
 * \brief Simple widget that provides functionality to add quotes to database.
 */
class CreateQuoteWidget : public QFrame
{
    Q_OBJECT
public:
    explicit CreateQuoteWidget(QWidget* parent = nullptr);
    ~CreateQuoteWidget();

private slots:
    /*!
     * \brief Adjust minimum possible size of chat message.
     * \param size - New size of the document in message. In pixels.
     */
    void _AdjustMinimumSize(const QSizeF &size);
    /*!
     * \brief Add quote that was typed in text field.
     */
    void _AddQuote();

private:
    /*! Layout for widgets */
    QHBoxLayout* _layout;
    /*! Add button */
    QPushButton* _addButton;
    /*! Text field */
    Common::EnhTextEdit*   _quoteText;
};

}
}
