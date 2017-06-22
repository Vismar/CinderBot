/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#include "PageListWidget.hpp"

using namespace Ui;

///////////////////////////////////////////////////////////////////////////

PageListWidget::PageListWidget(QWidget *parent) : QFrame(parent)
{
    // For borders
    this->setFrameShape(QFrame::Box);

    // Initialize main layout
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setAlignment(Qt::AlignTop);
    _InitializeHeader();
    _InitializeContent();
}

///////////////////////////////////////////////////////////////////////////

PageListWidget::~PageListWidget() {}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::SelectPage(int page)
{
    if (page <= _pageSlider->maximum())
    {
        _pageSlider->setValue(page);
    }
    else
    {
        _pageSlider->setValue(_pageSlider->maximum());
    }
}

///////////////////////////////////////////////////////////////////////////

int PageListWidget::GetCurrentPage() const
{
    return _pageSlider->value();
}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::UpdateIds(const QVector<int> &newIds)
{
    _ids = newIds;
    _UpdateSlider();
    _UpdatePageNumber();
    _UpdateContent();
}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::OnNumberOfEntriesPerPageChanged(const QString &newValue)
{
    Q_UNUSED(newValue);
    _UpdateSlider();
    _UpdatePageNumber();
    _UpdateContent();
}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::OnSliderMoved(int newPage)
{
    _pageSlider->setValue(newPage);
    _UpdatePageNumber();
    _UpdateContent();
}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::_AddWidget(QWidget *widget)
{
    _contentLayout->addWidget(widget);
}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::_InitializeHeader()
{
    // Layout and container
    _header = new QWidget();
    _headerLayout = new QHBoxLayout(_header);
    _mainLayout->addWidget(_header);

    // Entries per page
    _numberOfEntriesPerPage = new QComboBox();
    _numberOfEntriesPerPage->addItem("5");
    _numberOfEntriesPerPage->addItem("10");
    _numberOfEntriesPerPage->addItem("20");
    _numberOfEntriesPerPage->addItem("40");
    _headerLayout->addWidget(_numberOfEntriesPerPage);
    connect(_numberOfEntriesPerPage, &QComboBox::currentTextChanged,
            this, &PageListWidget::OnNumberOfEntriesPerPageChanged);

    // Page slider
    _pageSlider = new QSlider(this);
    _pageSlider->setOrientation(Qt::Horizontal);
    _pageSlider->setMinimum(1);
    _pageSlider->setSingleStep(1);
    _headerLayout->addWidget(_pageSlider);
    connect(_pageSlider, &QSlider::sliderMoved,
            this, &PageListWidget::OnSliderMoved);
    _UpdateSlider();

    // Page number
    _pageNumber = new QLabel(this);
    _headerLayout->addWidget(_pageNumber);
    _UpdatePageNumber();

}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::_InitializeContent()
{
    _content = new QScrollArea(this);
    _content->setWidgetResizable(true);
    QWidget *widget = new QWidget();
    _content->setWidget(widget);
    _contentLayout = new QVBoxLayout(widget);
    _contentLayout->setMargin(10);
    _contentLayout->setAlignment(Qt::AlignTop);
    _mainLayout->addWidget(_content);
}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::_UpdateSlider()
{
    // Get number of pages
    int numberOfPages = _ids.size()/_numberOfEntriesPerPage->currentText().toInt();
    if (_ids.size()%_numberOfEntriesPerPage->currentText().toInt())
    {
        ++numberOfPages;
    }
    if (numberOfPages == 0)
    {
        ++numberOfPages;
    }
    // Update maximum value of slider
    _pageSlider->setMaximum(numberOfPages);
    // Check if value is larger than maximum
    if (_pageSlider->value() > _pageSlider->maximum())
    {
        _pageSlider->setValue(_pageSlider->maximum());
    }
}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::_UpdatePageNumber()
{
    _pageNumber->setText(QString("Page %1/%2").arg(_pageSlider->value()).arg(_pageSlider->maximum()));
}

///////////////////////////////////////////////////////////////////////////

void PageListWidget::_UpdateContent()
{
    // Get number of entries which should be displayed on current page
    int numberOfEntriesPerPage = _numberOfEntriesPerPage->currentText().toInt();
    int numberOfEntries = _ids.size()-(numberOfEntriesPerPage*(_pageSlider->value()-1));
    if (numberOfEntries > numberOfEntriesPerPage)
    {
        numberOfEntries = numberOfEntriesPerPage;
    }
    // If current page should have less entries, remove redundant widgets
    if (_contentLayout->count() > numberOfEntries)
    {
        for (int i = _contentLayout->count(); i > numberOfEntries; --i)
        {
            QLayoutItem* item = _contentLayout->itemAt(i-1);
            if (item != NULL)
            {
                _contentLayout->removeItem(item);
                delete item->widget();
                delete item;
            }
        }
    }
    // If current page should have more entries, add missing widgets
    else if (_contentLayout->count() < numberOfEntries)
    {
        for (int i = _contentLayout->count()+1; i <= numberOfEntries; ++i)
        {
            _CreateAndAddWidget();
        }
    }
    // Update values of entries on current page
    for (int i = 0; i < _contentLayout->count(); ++i)
    {
        QLayoutItem* item = _contentLayout->itemAt(i);
        _UpdateEntry(item->widget(), _ids.at(numberOfEntriesPerPage*(_pageSlider->value()-1)+i));
    }
}

///////////////////////////////////////////////////////////////////////////
