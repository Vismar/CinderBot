/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include "ParametersCustomCommandWidget.hpp"

namespace Ui
{
namespace CustomCommand
{

class CreateCustomCommandWindow : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Constructor
     */
    explicit CreateCustomCommandWindow(QWidget *parent = 0);
    /*!
     * Destructor
     */
    ~CreateCustomCommandWindow();

signals:
    /*!
     * Signaling about new command that was created
     * \param cmdName - name of new command
     */
    void NewCommand(const QString &cmdName);

private slots:
    /*!
     * Check if command name field have text
     * \param text - new text in command name field
     */
    void _CheckCommandNameField(const QString &text);
    /*!
     * Handle "Create" button
     */
    void _OnCreateButton();

private:
    /*! Main layout of window */
    QVBoxLayout *_layout;
    /*! Parameters of a new custom command */
    ParametersCustomCommandWidget *_parameters;
    /*! Create button */
    QPushButton *_createButton;
};

}
}
