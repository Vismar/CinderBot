/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QLabel>
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
 * \brief This class provides functionality to edit/save and delete stored quote.
 */
class SingleQuoteWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SingleQuoteWidget(QWidget* parent = nullptr);
    ~SingleQuoteWidget();

    /*!
     * \brief Sets number field for quote.
     * \param number - number that should be setted.
     */
    void SetQuoteNumber(const QString &number) const;
    /*!
     * \brief Sets text field for quote.
     * \param text - text which will be used in text field.
     */
    void SetQuoteText(const QString &text) const;

private slots:
    /*!
     * \brief Adjust minimum possible size of chat message.
     * \param size - New size of the document in message. In pixels.
     */
    void _AdjustMinimumSize(const QSizeF &size);
    /*!
     * \brief Handles edit/save button.
     */
    void _HandleEditSaveButton() const;
    /*!
     * \brief Handles delete button.
     */
    void _HandleDeleteButton() const;

private:
    /*! Widget layout */
    QHBoxLayout* _layout;
    /*! Quote number field */
    QLabel*      _quoteNumber;
    /*! Quote text field */
    Common::EnhTextEdit*   _quoteText;
    /*! Quote edit/save button */
    QPushButton* _quoteEditSave;
    /*! Quote delete button */
    QPushButton* _quoteDelete;
};

}
}
