/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

namespace Ui
{

class QuotesWidget : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * Constructor
     * \param parent - parent widget
     */
    explicit QuotesWidget(QWidget* parent = 0);
    /*!
     * Destructor
     */
    ~QuotesWidget();

public slots:
    /*!
     * Update quotes. Create or delete widgets, then update quote numbers and text fields
     * \param tableName - name of table
     */
    void UpdateQuotes(const QString& tableName);

private:
    /*! Vertical layout */
    QVBoxLayout* _layout;
    /*! Simple container to store scrol layout */
    QWidget*     _container;
};

}
