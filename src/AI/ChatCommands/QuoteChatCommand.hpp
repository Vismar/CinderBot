#pragma once
#include "BaseChatCommand.hpp"
#include <QVector>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

/*!
 * Class QuoteChatCommand
 * Chatt command that handles quotes.
 * It can display random quote if user typed #quote, but if he specified id of quote
 * like #quote N, where N is id of quote, bot will try to find quote with this id
 * and display it, otherwise random quote will be returned.
 * Also new quotes can be added through chat via command #add_quote.
 * All commands can be modified via command #edit_command, or deleted via #delete_command.
 */
class QuoteChatCommand : public BaseChatCommand
{
private:
    /*! List of quotes */
    QVector<QString> _quotes;
    /*! Xml stream reader */
    QXmlStreamReader _xmlReader;
    /*! Xml stream writer */
    QXmlStreamWriter _xmlWriter;

    /*!
     * Try to initialize quotes from .xml file
     */
    void _Initialize();
    /*!
     * Save all quotes to .xml file
     */
    void _SaveQuotes();
    /*!
     * Try to get number right after command.
     * \param(IN) command - command itself
     * \param(IN) message - message in which number should be found
     * \param(OUT) value - number that was found
     * \return true, if number was found
     */
    bool _GetNumberAfterCommand(const QString& command, const QString& message, QString& value);

public:
    QuoteChatCommand();
    ~QuoteChatCommand();
    ////////////////////////////////
    /// BaseChatCommand override
    bool GetAnswer(const ChatMessage& message, QString& answer);
};
