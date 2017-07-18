/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QLineEdit>

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
 * \brief Enhancement QLineEdit.
 *
 * EnhLineEdit includes some additional features for QLineEdit that are required by multiple places in code.
 */
class EnhLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit EnhLineEdit(QWidget* parent = 0);
    virtual ~EnhLineEdit();

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
