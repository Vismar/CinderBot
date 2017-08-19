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
 * \brief Common widgets that required by multiple other windows and widgets.
 */
namespace Common
{

/*!
 * \brief Simple class which have text field and two buttons: edit/save and delete.
 * 
 * Creates specifally for places where we need to dusplay simeple text field and 2 buttons.
 * If edit was clicked, then button will be switched to "save" state.
 */
class EditSaveWidget : public QFrame
{
    Q_OBJECT
public:
    explicit EditSaveWidget(QWidget* parent = 0);
    virtual ~EditSaveWidget();

    /*!
     * \brief Sets id for entry.
     * \param id - id of an entry.
     * 
     * Simple and internal thing.
     */
    void SetId(int id);
    /*!
     * \brief Sets text field.
     * \param text - text which will be used in text field.
     */
    void SetText(const QString &text) const;

protected:
    /*!
     * \brief Save current entry.
     * 
     * Called to when "Save" button was pressed. Should be implemented by inherited classes.
     */
    virtual void _SaveEntry() = 0;
    /*!
     * \brief Delete current entry
     * 
     * Called to when "Delete" button was pressed. Should be implemented by inherited classes.
     */
    virtual void _DeleteEntry() = 0;

    /*! Id of entry */
    int _id;
    /*! Text field */
    EnhTextEdit*   _text;

private slots:
    /*!
     * \brief Adjust minimum possible size of text field.
     * \param size - New size of the document in message. In pixels.
     */
    void _AdjustMinimumSize(const QSizeF &size);
    /*!
     * \brief Handles edit/save button.
     * 
     * Switches states and handles presses of "Edit" and "Save" buttons.
     */
    void _HandleEditSaveButton();
    /*!
     * \brief Handles delete button.
     * 
     * Deletes entry by calling _DeleteNetry().
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
}
