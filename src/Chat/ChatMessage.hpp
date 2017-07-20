/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QString>
#include <QRegularExpression>

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
    ROOMSTATE,
    JOIN,
    PART,
    MODE,
    UNMODE,
    PRIVMSG,
    WHISPER,
    BITS
};

/*!
 * Class ChatMessage
 * Container for message data. Also handle parsing of raw data.
 */
class ChatMessage
{
public:
    ChatMessage();
    /*** Get functions ***/
    /*!
     * Returns author name
     * \return author name
     */
    const QString &GetAuthor() const;
    /*!
     * Returns real user name
     * \return real user name
     */
    const QString &GetRealName() const;
    /*!
     * Returns name color
     * \return name color
     */
    const QString &GetColor() const;
    /*!
     * Returns timestamp of message
     * \return timestamp
     */
    const QString &GetTimeStamp() const;
    /*!
     * Returns messsage
     * \return message
     */
    const QString &GetMessage() const;
    /*!
     * Check if author is a moderator
     * \return moderator status
     */
    bool IsModerator() const;
    /*!
     * Check if author is a broadcaster
     * \return true if author name equals channel name
     */
    bool IsBroadcaster() const;
    /*!
     * Return type of message
     * \return type of message that was assigned during the parsing
     */
    MessageType GetType() const;
    /*!
     * \brief Gets number of bits.
     * \return string which contains bits
     *
     * Gets number of bits as string that was transfered through message.
     */
    const QString &GetBits() const;

    /*** Manual set functions ***/
    /*!
     * Set author name
     * \param(IN) author - Author name
     */
    void SetAuthor(const QString &author);
    /*!
     * Set real user name
     * \param(IN) realName - Real user name
     */
    void SetRealName(const QString &realName);
    /*!
     * Set author's name color
     * \param(IN) color - custom name color
     */
    void SetColor(const QString &color);
    /*!
     * Set message data
     * \param(IN) message - message itself
     */
    void SetMessage(const QString &message);
    /*!
     * Set moderator flag
     * \param modFlag
     */
    void SetModFlag(bool modFlag);
    /*!
     * Set type of message
     * \param type - type which will be setted
     */
    void SetType(MessageType type);
    /*!
     * \brief Sets specified number of bits.
     * \param bits - number of bits as string.
     *
     * Manually sets number of bits to message.
     */
    void SetBits(const QString &bits);

    /*!
     * Parse raw message data
     * \param(IN) message - raw message data
     * \return message type
     */
    MessageType ParseRawMessage(const QString &message);

private:
    /*** Check functions ***/
    /*!
     * Check message if it is a network message
     * \param(IN) message - message to check
     * \return true, if it is a network message
     */
    bool _IsNetworkMsg(const QString &message) const;
    /*!
     * Check message if it is a ping command
     * \param(IN) message - message to check
     * \return true, if it is a ping command
     */
    bool _IsPingCommand(const QString &message) const;
    /*!
     * Check message if it is a pong command
     * \param(IN) message - message to check
     * \return true, if it is a pong command
     */
    bool _IsPongCommand(const QString &message) const;

    /*!
     * Check message if it is an IRC command
     * \param(IN) message - message to check
     * \return IRC code
     */
    int  _IsIrcComand(const QString &message) const;
    /*!
     * Check message if it about connection to room
     * \param(IN) message - message to check
     * \return true, if it about connection to room
     */
    bool _IsConnectedToRoom(const QString &message) const;
    /*!
     * Check message if it is a userstate message
     * \param(IN) message - message to check
     * \return true, if it is a userstate message
     */
    bool _IsUserState(const QString &message) const;
    /*!
     * \brief Chechs if current message is a roomstate message.
     * \param message - message to check.
     * \return true, if it is a roomstate message.
     *
     * Checks if current message matches room state regular expression.
     */
    bool _IsRoomState(const QString &message) const;
    /*!
     * Check message if it is a channel message
     * \param(IN) message - message to check
     * \return true, if it is a channel message
     */
    bool _IsPrivMsg(const QString &message) const;
    /*!
     * Check message if it is a join message
     * \param(IN) message - message to check
     * \return true, if it is a join message
     */
    bool _IsJoinMsg(const QString &message) const;
    /*!
     * Check message if it is a part message
     * \param(IN) message - message to check
     * \return true, if it is a part message
     */
    bool _IsPartMsg(const QString &message) const;
    /*!
     * Check if message means that someone was setted as mode in the chat
     * \param(IN) message - message to check
     * \return true, if it is a mode message
     */
    bool _IsModeMessage(const QString &message) const;
    /*!
     * Check if message means that someone was downgraded from mode in the chat
     * \param(IN) message - message to check
     * \return true, if it is a unmode message
     */
    bool _IsUnmodeMessage(const QString &message) const;
    /*!
     * Check if it is whisper message
     * \param message - message to check
     * \return true if it is a whisper message
     */
    bool _IsWhisper(const QString &message) const;
    /*!
     * \brief Checks if current messages contains bits.
     * \param message - message to check
     * \return true if it is a bits message
     *
     * Checks if current message matches bits regular expression.
     */
    bool _IsBits(const QString &message) const;

    /*** Set functions ***/
    /*!
     * Set timestamp
     */
    void _SetTimeStamp();
    /*!
     * Parse message and get custom name color, then set it
     * \param(IN) message - message to parse
     */
    void _GetAndSetNameColor(const QString &message);
    /*!
     * Parse message and get name of user, then set it
     * \param(IN) message - message to parse
     */
    void _GetAndSetAuthor(const QString &message);
    /*!
     * Parse message and get name of user of moderator message, then set it
     * \param(IN) message - message to parse
     */
    void _GetAndSetAuthorForMode(const QString &message, MessageType msgType);
    /*!
     * Parse message and get message itself, then set it
     * \param(IN) message - message to parse
     */
    void _GetAndSetChatMessage(const QString &message);
    /*!
     * Parse message and get moderator flag, then set it
     * \param(IN) message - message to parse
     */
    void _GetAndSetModeratorFlag(const QString &message);
    /*!
     * \brief Parse message and get number of bits.
     * \param message - message to parse.
     *
     * Parse message to find needed tag and get number of bits that were given through message.
     */
    void _GetAndSetBits(const QString &message);

    /*** Chat message parameters ***/
    /*! Author of message*/
    QString _author;
    /*! Real user name */
    QString _realName;
    /*! Custom name color of author if it was set */
    QString _color;
    /*! Timestamp of a moment when message was received */
    QString _timeStamp;
    /*! Message itself */
    QString _message;
    /*! Moderator flag */
    bool _isModerator;
    /*! Type of the message */
    MessageType _type;
    /*! Bits */
    QString _bits;

    /*** Regular expressions ***/
    static QRegularExpression _regExpPing;
    static QRegularExpression _regExpPong;
    static QRegularExpression _regExpUserstate;
    static QRegularExpression _regExpRoomState;
    static QRegularExpression _regExpJoin;
    static QRegularExpression _regExpPart;
    static QRegularExpression _regExpMode;
    static QRegularExpression _regExpUnmode;
    static QRegularExpression _regExpPrivmsg;
    static QRegularExpression _regExpWhisper;
    static QRegularExpression _regExpBits;
};
