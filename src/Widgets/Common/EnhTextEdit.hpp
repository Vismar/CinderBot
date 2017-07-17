/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QTextEdit>

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
 * \brief Enhancement EnhTextEdit.
 *
 * EnhTextEdit includes some additional features for QTextEdit that are required by multiple places in code.
 */
class EnhTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit EnhTextEdit(QWidget* parent = 0);
    virtual ~EnhTextEdit();

    /*!
     * \brief Set editable mode to specific state.
     * \param state - flag that turns editable mode on or off.
     *
     * Turns on/off readl only mode and changes stylesheet.
     */
    void SetEditable(bool state);
};

}
}
