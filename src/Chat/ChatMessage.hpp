#pragma once
#include <QString>

#define SYSTEM_MESSAGE    "System Message"
#define SYSTEM_MESSAGE_CR "Connected to the room!"

/*!
 * MessageType
 * Type of message
 */
enum MessageType
{
    Undefined = -1,
    INFO,
    PING,
    PONG,
    LOGIN_OK,
    USERSTATE,
    PRIVMSG
};

/*!
 * Class ChatMessage
 * Container for message data. Also handle parsing of raw data.
 */
class ChatMessage
{
private:
    /*! Author of message*/
    QString _author;
    /*! Custom name color of author if it was set */
    QString _color;
    /*! Timestamp of a moment when message was received */
    QString _timeStamp;
    /*! Message itself */
    QString _message;
    /*! Moderator flag */
    bool    _isModerator;

    /*!
     * Check message if it is a network message
     * \param(IN) message - message to check
     * \return true, if it is a network message
     */
    bool _IsNetworkMsg(const QString& message) const;
    /*!
     * Check message if it is a ping command
     * \param(IN) message - message to check
     * \return true, if it is a ping command
     */
    bool _IsPingCommand(const QString& message) const;
    /*!
     * Check message if it is a pong command
     * \param(IN) message - message to check
     * \return true, if it is a pong command
     */
    bool _IsPongCommand(const QString& message) const;

    /*!
     * Check message if it is an IRC command
     * \param(IN) message - message to check
     * \return IRC code
     */
    int  _IsIrcComand(const QString& message) const;
    /*!
     * Check message if it about connection to room
     * \param(IN) message - message to check
     * \return true, if it about connection to room
     */
    bool _IsConnectedToRoom(const QString& message) const;
    /*!
     * Check message if it is a userstate message
     * \param(IN) message - message to check
     * \return true, if it is a userstate message
     */
    bool _IsUserState(const QString& message) const;
    /*!
     * Check message if it is a channel message
     * \param(IN) message - message to check
     * \return true, if it is a channel message
     */
    bool _IsPrivMsg(const QString& message) const;
    /*!
     * Set timestamp
     */
    void _SetTimeStamp();

public:
    ChatMessage();
    /*!
     * Returns author name
     * \return author name
     */
    const QString& GetAuthor() const;
    /*!
     * Returns name color
     * \return name color
     */
    const QString& GetColor() const;
    /*!
     * Returns timestamp of message
     * \return timestamp
     */
    const QString& GetTimeStamp() const;
    /*!
     * Returns messsage
     * \return message
     */
    const QString& GetMessage() const;
    /*!
     * Check if author is a moderator
     * \return moderator status
     */
    bool           IsModerator() const;

    /*!
     * Set author name
     * \param(IN) author - Author name
     */
    void SetAuthor(const QString& author);
    /*!
     * Set author's name color
     * \param(IN) color - custom name color
     */
    void SetColor(const QString& color);
    /*!
     * Set message data
     * \param(IN) message - message itself
     */
    void SetMessage(const QString& message);
    /*!
     * Parse raw message data
     * \param(IN) message - raw message data
     * \return message type
     */
    MessageType ParseRawMessage(const QString& message);
};
