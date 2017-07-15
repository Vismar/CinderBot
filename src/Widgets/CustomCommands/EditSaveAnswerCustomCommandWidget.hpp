/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "Widgets/Common/EditSaveWidget.hpp"

namespace Ui
{
namespace CustomCommand
{

class EditSaveAnswerCustomCommandWidget : public Common::EditSaveWidget
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit EditSaveAnswerCustomCommandWidget(QWidget* parent = 0);
    /*!
     * Destructor
     */
    virtual ~EditSaveAnswerCustomCommandWidget();

protected:
    ////////////////////////////////
    /// EditSaveWidget override
    void _SaveEntry();
    void _DeleteEntry();
};

}
}
