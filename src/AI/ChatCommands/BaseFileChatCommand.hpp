#pragma once
#include "BaseChatCommand.hpp"
#include "ChatCommand.hpp"
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
    QVector<ChatCommand> _commands;
    /*! Xml stream reader */
    QXmlStreamReader _xmlReader;

private:
    /*!
     * Try to initialize custom commands from file
     */
    void _Initialize();
};
