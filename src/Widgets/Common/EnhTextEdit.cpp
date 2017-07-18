/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "EnhTextEdit.hpp"

using namespace Ui::Common;

///////////////////////////////////////////////////////////////////////////

EnhTextEdit::EnhTextEdit(QWidget *parent) : QTextEdit(parent) {}

///////////////////////////////////////////////////////////////////////////

EnhTextEdit::~EnhTextEdit() {}

///////////////////////////////////////////////////////////////////////////

void EnhTextEdit::SetEditable(bool state)
{
    this->setReadOnly(!state);
    if (state)
    {
        this->setStyleSheet("");
    }
    else
    {
        this->setStyleSheet("QTextEdit { background-color: #f0f0f0; }");
    }
}

///////////////////////////////////////////////////////////////////////////
