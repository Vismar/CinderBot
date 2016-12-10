#pragma once
#include "../CommandList.hpp"

namespace Command
{

/*!
 * Class QuoteCommandList
 * List of quote commands
 */
class QuoteCommandList : public CommandList
{
public:
    /*! Constructor */
    QuoteCommandList();
    /*! Destructor */
    ~QuoteCommandList();

protected:
    /*!
     * Save all quotes to .xml file
     */
    void _SaveQuotes();

    ////////////////////////////////
    /// CommandList override
    void _Initialize();

    /*! Xml stream reader */
    QXmlStreamReader _xmlReader;
    /*! Xml stream writer */
    QXmlStreamWriter _xmlWriter;
    /*! List of quotes */
    QVector<QString> _quotes;
};

}
