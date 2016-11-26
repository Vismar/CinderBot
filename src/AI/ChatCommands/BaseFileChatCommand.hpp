#pragma once
#include "BaseChatCommand.hpp"
#include "BaseFileChatCommandContainer.hpp"
#include <QPair>
#include <QString>
#include <QVector>
#include <QXmlStreamReader>

/*!
 * Class BaseFileChatCommand
 * Stores custom commands that was defined user in special file.
 */
class BaseFileChatCommand : public BaseChatCommand
{
public:
    BaseFileChatCommand();
    ////////////////////////////////
    /// BaseChatCommand override
    bool GetAnswer(const ChatMessage& message, QString& answer);

protected:
    /*! List of custom commands */
    QVector<BFChatCommand> _commands;
    /*! Xml stream reader */
    QXmlStreamReader _xmlReader;

private:
    /*!
     * Try to initialize custom commands from file
     */
    void _Initialize();
    /*!
     * Try to read command and add it to the list
     */
    void _ReadCommand();
};
