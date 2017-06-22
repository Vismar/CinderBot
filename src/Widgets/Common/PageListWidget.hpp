/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QWidget>
#include <QVector>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>

namespace Ui
{

/*!
 * class PageListWidget
 * Common UI widget which displays any kind of data on "pages". It slicing data into pages
 * and provides functionality to change number of entries which will be displayed per page
 * and navigation through those pages.
 */
class PageListWidget : public QFrame
{
    Q_OBJECT
public:
    /*!
     * Constructor
     */
    explicit PageListWidget(QWidget *parent = 0);
    /*!
     * Destructor
     */
    virtual ~PageListWidget();

    /*!
     * Select specified page
     * \param page - page number which should be selected
     */
    void SelectPage(int page);
    /*!
     * Get current page number
     * \return current page number
     */
    int GetCurrentPage() const;

public slots:
    /*!
     * Slot to update whole widget when array of ids was changed
     * \param newIds - reference to array of new ids
     */
    void UpdateIds(const QVector<int> &newIds);

protected:
    /*!
     * Abstract function to create specific widget
     */
    virtual void _CreateAndAddWidget() = 0;
    /*!
     * Update specified widget with data related to specified id
     * \param entry - pointer to widget which should be updated
     * \param id - data id
     */
    virtual void _UpdateEntry(QWidget *entry, int id) = 0;
    /*!
     * Add widget to content area
     * \param widget - pointer to widget which will be added to conent area
     */
    void _AddWidget(QWidget *widget);

private slots:
    /*!
     * Slot to update widget when number of entries per page was changed
     * \param newValue - not used
     */
    void OnNumberOfEntriesPerPageChanged(const QString &newValue);
    /*!
     * Slot to update widget when page slider was moved
     * \param newPage - new page value
     */
    void OnSliderMoved(int newPage);

private:
    /*!
     * Initialize widgets which placed in header
     */
    void _InitializeHeader();
    /*!
     * Inititalize widgets that placed in content area
     */
    void _InitializeContent();

    /*!
     * Update page slider
     */
    void _UpdateSlider();
    /*!
     * Update page number label
     */
    void _UpdatePageNumber();
    /*!
     * Update content area
     */
    void _UpdateContent();

    /*! Main layout for widget */
    QVBoxLayout *_mainLayout;

    /*** Header ***/
    /*! Header container */
    QWidget *_header;
    /*! Layout for header widgets */
    QHBoxLayout *_headerLayout;
    /*! Simple label for text "Per page" */
    QLabel *_perPage;
    /*! ComboBox for header */
    QComboBox *_numberOfEntriesPerPage;
    /*! Page slider */
    QSlider *_pageSlider;
    /*! Label for "Page number i/n" */
    QLabel *_pageNumber;

    /*** Content ***/
    /*! Area for content */
    QScrollArea *_content;
    /*! Layout for content area */
    QVBoxLayout *_contentLayout;
    /*! Array of ids */
    QVector<int> _ids;
};

}
