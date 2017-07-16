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
#include <QPushButton>

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
 * \brief Common UI widget which displays any kind of data on "pages".
 *
 * This widget store array of ids and dynamically updates it. Mainly, widget slices data
 * into pages and provides functionality to change number of entries which will be
 * displayed per page and navigation through those pages.
 */
class PageListWidget : public QFrame
{
    Q_OBJECT
public:
    explicit PageListWidget(QWidget *parent = 0);
    virtual ~PageListWidget();

    /*!
     * \brief Selects specified page.
     * \param page - page number which should be selected.
     *
     * Selects specified page and updates content of widget.
     */
    void SelectPage(int page);
    /*!
     * Get current page number.
     * \return current page number
     *
     * Returns page number which is currently selected.
     */
    int GetCurrentPage() const;

public slots:
    /*!
     * \brief Slot to update whole widget when array of ids was changed.
     * \param newIds - reference to array of new ids.
     *
     * Replaces old id arra by new one and updates the whole data in widget.
     */
    void UpdateIds(const QVector<int> &newIds);

protected:
    /*!
     * \brief Abstract function to create specific widget.
     *
     * This function should be implemented by inherited class.
     * This function is called when PagedListWidget need to
     * create additional entries to display content.
     */
    virtual void _CreateAndAddWidget() = 0;
    /*!
     * \brief Update specified widget with data related to specified id.
     * \param entry - pointer to widget which should be updated.
     * \param id - data id.
     *
     * This function should be implemented by inherited class.
     * Tiis function is called when PagedListWidget need to update value of specified entry.
     */
    virtual void _UpdateEntry(QWidget *entry, int id) = 0;
    /*!
     * \brief Add widget to content area.
     * \param widget - pointer to widget which will be added to conent area.
     *
     * Add specific widget to content area.
     */
    void _AddWidget(QWidget *widget);

private slots:
    /*!
     * \brief Slot to update widget when number of entries per page was changed.
     * \param newValue - not used.
     *
     * This slot called when number of entries per page was changed.
     */
    void _OnNumberOfEntriesPerPageChanged(const QString &newValue);
    /*!
     * \brief Slot to update widget when page slider was moved.
     * \param newPage - new page value.
     *
     * This slot called when value of slider was changed.
     */
    void _OnSliderMoved(int newPage);
    /*!
     * \brief Lists content to the previous page if possible.
     *
     * Checks if page is not first, if not - selects previous page.
     */
    void _OnPrevPageButtonPress();
    /*!
     * \brief Lists content to the next page if possible.
     *
     * Checks if page is not last, if not - selects next page.
     */
    void _OnNextPageButtonPress();

private:
    /*!
     * \brief Initializes widgets which placed in header.
     *
     * Inititalizes widgets which were placed in header such is QComboBox, QSlider and two labels.
     */
    void _InitializeHeader();
    /*!
     * \brief Initialize widgets that placed in content area.
     *
     *  Initializes conten widgets. Calls virtual functions that should be implemented.
     */
    void _InitializeContent();

    /*!
     * \brief Update page slider.
     *
     * Sets new max number of pages that available, displays last page
     * if previously selected page does not exist anymore.
     */
    void _UpdateSlider();
    /*!
     * \brief Update page number label.
     *
     * Sets new value to page number label.
     */
    void _UpdatePageNumber();
    /*!
     * \brief Update content area.
     *
     * Updates all widgets in conten area.
     */
    void _UpdateContent();

    /*! Main layout for widget */
    QVBoxLayout *_mainLayout;

    /*** Header ***/
    /*! Header container */
    QWidget *_header;
    /*! Layout for header widgets */
    QHBoxLayout *_headerLayout;
    /*! ComboBox for header */
    QComboBox *_numberOfEntriesPerPage;
    /*! Button to select previous page */
    QPushButton *_prevPage;
    /*! Page slider */
    QSlider *_pageSlider;
    /*! Button to select next page */
    QPushButton *_nextPage;
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
}
