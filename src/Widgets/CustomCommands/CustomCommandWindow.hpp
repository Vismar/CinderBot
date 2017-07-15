/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QVBoxLayout>
#include <QPushButton>
#include "Widgets/CustomCommands/ListCustomCommandWidget.hpp"

namespace Ui
{
namespace CustomCommand
{

class CustomCommandWindow : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructor
     */
    explicit CustomCommandWindow(QWidget *parent = 0);
    /*!
     * Destructor
     */
    ~CustomCommandWindow();

private slots:
    /*!
     * Open dialog box for creating new command
     */
    void _CreateCommand();
    /*!
     * Update command list
     * \param tableName - table name which was updated
     */
    void _UpdateCommands(const QString& tableName);
    /*!
     * Open new window to edit new command
     * \param cmdName - command name which should be edited
     */
    void _EditNewCommand(const QString &cmdName);

private:
    /*! Main layout of window */
    QVBoxLayout *_mainLayout;
    /*! Create button */
    QPushButton *_createButton;
    /*! List of commands */
    ListCustomCommandWidget *_commandList;
};

}
}
