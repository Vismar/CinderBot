/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include "ParametersCustomCommandWidget.hpp"
#include "AnswersCustomCommandWidget.hpp"

/*!
* \brief All Ui widgets and windows.
*/
namespace Ui
{
 /*!
  * \brief All widgets related to custom commands.
  */
namespace CustomCommand
{

/*!
 * \brief WIndow to edit custom command.
 * 
 * Widget/window which displays all custom command parameters and has capability to view/edit/delete/add answers.
 */
class EditCustomCommandWindow : public QDialog
{
    Q_OBJECT
public:
    explicit EditCustomCommandWindow(QWidget *parent = 0);
    ~EditCustomCommandWindow();

    /*!
     * \brief Loads data related to specified command.
     * \param cmdName - name of command.
     * 
     * Gets all params form CustomCommandDBHelper, sets it to widget 
     * and generates answer ids to populate them in AnswersCustomCommandWidget.
     */
    void LoadCommandData(const QString& cmdName);

private slots:
    /*!
     * \brief Updates "Add" button to be active or not.
     * 
     * If answer field has at least 1 symbol activates "Add" button. Otherwise, disables it.
     */
    void _CheckAnswerField() const;
    /*!
     * \brief Handle "Add" button.
     * 
     * Adds new answer to current command.
     */
    void _OnAddButton() const;
    /*!
     * \brief Handle "Save" button.
     * 
     * Gets all params from widget and uses them to updates current command.
     */
    void _OnSaveButton();
    /*!
     * \brief Update whole list of answers.
     * 
     * Loads answer ids from CustomCommandDBHelper and uses them to initialize answers for current command.
     */
    void _LoadAnswers() const;
    /*!
     * \brief Adds new answer to answer list.
     *
     * Calls _LoadAnswers() to update answer list of current command.
     */
    void _AddAnswer(const QString &cmdName) const;
    /*!
     * \brief Deletes specific answer from answer list.
     *
     * Deletes id from answer list and updates AnswerCustomCommandWidget.
     */
    void _DeleteAnswer(const QString &cmdName, int id) const;

private:
    /*! Main list of window */
    QHBoxLayout *_mainLayout;
    /*! Left part of window */
    QFrame *_leftPart;
    /*! Layout for left part of window */
    QVBoxLayout *_leftLayout;
    /*! Parameters of command */
    ParametersCustomCommandWidget *_parameters;
    /*! Text field of new answer */
    QPlainTextEdit *_newAnswer;
    /*! Add button */
    QPushButton *_addButton;
    /*! Save button */
    QPushButton *_saveButton;
    /*! List of answers */
    AnswersCustomCommandWidget *_answers;
};

}
}
