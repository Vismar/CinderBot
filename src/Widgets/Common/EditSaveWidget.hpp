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

/*!
 * class EditSaveWidget
 * Simple class which have text field and two buttons: edit/save and delete.
 * If edit was clicked, then button will be switched to "save" state.
 */
class EditSaveWidget : public QFrame
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit EditSaveWidget(QWidget* parent = 0);
    /*!
     * Destructor
     */
    virtual ~EditSaveWidget();

    /*!
     * Set number field for quote
     * \param number - number that should be setted
     */
    void SetId(int id);
    /*!
     * Set text field for quote
     * \param text - text which will be used in text field
     */
    void SetText(const QString &text);

protected:
    /*!
     * Save current entry
     */
    virtual void _SaveEntry() = 0;
    /*!
     * Delete current entry
     */
    virtual void _DeleteEntry() = 0;

    /*! Id of entry */
    int _id;
    /*! Text field */
    QTextEdit*   _text;

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
    /*! State of button */
    bool _editState;
    /*! Edit/save button */
    QPushButton* _editSaveButton;
    /*! Delete button */
    QPushButton* _deleteButton;
};

}
