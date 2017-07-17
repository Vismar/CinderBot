/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EnhLineEdit.hpp"

using namespace Ui::Common;

///////////////////////////////////////////////////////////////////////////

EnhLineEdit::EnhLineEdit(QWidget *parent) : QLineEdit(parent) {}

///////////////////////////////////////////////////////////////////////////

EnhLineEdit::~EnhLineEdit() {}

///////////////////////////////////////////////////////////////////////////

void EnhLineEdit::SetEditable(bool state)
{
    this->setReadOnly(!state);
    if (state)
    {
        this->setStyleSheet("");
    }
    else
    {
        this->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");
    }
}

///////////////////////////////////////////////////////////////////////////
