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
 * \brief Contains all things for chat.
 */
namespace Chat
{

/*!
 * \brief Message type enumeration.
 *
 * Contains all types of messages that can be received from twitch.tv.
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
    BITS,
    PRIVMSG,
    WHISPER
};

/*!
 * \brief Container for message data.
 *
 * Parses raw message and stores needed data to parameters.
 */
class ChatMessage
{
public:
    ChatMessage();
    /*** Get functions ***/
    /*!
     * \brief Returns author name.
     * \return author name.
     *
     * Returns saved display name of the user which sent this message.
     */
    const QString &GetAuthor() const;
    /*!
     * \brief Returns real user name.
     * \return real user name.
     *
     * Returns login name of the user which sent this message.
     */
    const QString &GetRealName() const;
    /*!
     * \brief Returns name color.
     * \return name color.
     *
     * Returns name color of the user which sent this message.
     */
    const QString &GetColor() const;
    /*!
     * \brief Returns time-stamp of message.
     * \return time-stamp.
     *
     * Returns time-stamp for this message
     */
    const QString &GetTimeStamp() const;
    /*!
     * \brief Returns message.
     * \return message.
     *
     * Returns message which sent.
     */
    const QString &GetMessage() const;
    /*!
     * \brief Checks if author is a moderator.
     * \return moderator status.
     *
     * Checks if user which sent this message is a moderator.
     */
    bool IsModerator() const;
    /*!
     * \brief Checks if author is a broadcaster.
     * \return true if author name equals channel name.
     *
     * Checks if user which sent this message is the broadcaster.
     */
    bool IsBroadcaster() const;
    /*!
     * \brief Returns type of message.
     * \return type of message that was assigned during the parsing.
     *
     * Returns type of message that was set after parsing raw data.
     */
    MessageType GetType() const;
    /*!
     * \brief Gets number of bits.
     * \return string which contains bits.
     *
     * Gets number of bits as string that was transfered through message.
     */
    const QString &GetBits() const;
    /*!
     * \brief Check if author is a subscriber.
     * \return subscriber status.
     *
     * Check if user which sent this message is a subscriber.
     */
    bool IsSubscriber() const;
    /*!
     * \brief Get user id.
     * \return Id of user.
     */
    int GetUserID() const;

    /*** Manual set functions ***/
    /*!
     * \brief Sets author name.
     * \param author - Author name.
     *
     * Sets manually display name to this message.
     */
    void SetAuthor(const QString &author);
    /*!
     * \brief Sets real user name.
     * \param realName - Real user name.
     *
     * Sets manually login name to this message.
     */
    void SetRealName(const QString &realName);
    /*!
     * \brief Sets author's name color.
     * \param color - custom name color.
     *
     * Sets manually name color to this message.
     */
    void SetColor(const QString &color);
    /*!
     * \brief Sets message data.
     * \param message - message itself.
     *
     * Set manually message data.
     */
    void SetMessage(const QString &message);
    /*!
     * \brief Sets moderator flag.
     * \param modFlag - moderation flag.
     *
     * Sets manually moderation flag to this message.
     */
    void SetModFlag(bool modFlag);
    /*!
     * \brief Sets type of message.
     * \param type - type which will be set.
     *
     * Sets manually type to this message.
     */
    void SetType(MessageType type);
    /*!
     * \brief Sets specified number of bits.
     * \param bits - number of bits as string.
     *
     * Sets manually number of bits to message.
     */
    void SetBits(const QString &bits);
    /*!
     * \brief Sets subscriber status.
     * \param subFlag - status of subscription.
     *
     * Sets manually subscriber status to this message.
     */
    void SetSubscriber(bool subFlag);
    /*!
     * \brief Sets user id..
     * \param userID - id of user.
     */
    void SetUserID(int userID);

    /*!
     * \brief Parse raw message data.
     * \param message - raw message data.
     * \return message type.
     *
     * Parse raw message data that was got from twitch.tv and store needed params.
     */
    MessageType ParseRawMessage(const QString &message);

private:
    /*** Check functions ***/
    /*!
     * \brief Checks message if it is a network message.
     * \param message - message to check.
     * \return true, if it is a network message.
     *
     * Checks if raw message is network message.
     */
    bool _IsNetworkMsg(const QString &message) const;
    /*!
     * \brief Checks message if it is a ping command.
     * \param message - message to check.
     * \return true, if it is a ping command.
     *
     * Checks if raw message is ping command from twitch.tv.
     */
    bool _IsPingCommand(const QString &message) const;
    /*!
     * \brief Checks message if it is a pong command.
     * \param message - message to check.
     * \return true, if it is a pong command.
     *
     * Checks if raw message is pong command from twitch.tv.
     */
    bool _IsPongCommand(const QString &message) const;

    /*!
     * \brief Checks message if it is an IRC command.
     * \param message - message to check.
     * \return IRC code.
     *
     * Checks if raw message is irc command from twitch.tv.
     */
    int  _IsIrcComand(const QString &message) const;
    /*!
     * \brief Check message if it about connection to room.
     * \param message - message to check.
     * \return true, if it about connection to room.
     *
     * Checks if raw message is connected to room event.
     */
    bool _IsConnectedToRoom(const QString &message) const;
    /*!
     * \brief Check message if it is a userstate message.
     * \param message - message to check.
     * \return true, if it is a userstate message.
     *
     * Checks if current message matches userstate regular expression,
     * saves bot display name, moderation status and name color that was set on twitch.
     */
    bool _IsUserState(const QString &message);
    /*!
     * \brief Checks if current message is a roomstate message.
     * \param message - message to check.
     * \return true, if it is a roomstate message.
     *
     * Checks if current message matches room state regular expression.
     */
    bool _IsRoomState(const QString &message) const;
    /*!
     * \brief Check message if it is a join message.
     * \param message - message to check.
     * \return true, if it is a join message.
     *
     * Checks if raw message is join message from twitch.tvt that notify about new user in chat.
     */
    bool _IsJoinMsg(const QString &message);
    /*!
     * \brief Check message if it is a part message.
     * \param message - message to check.
     * \return true, if it is a part message.
     *
     * Checks if raw message is part message from twitch.tvt that notify about user that leaves chat.
     */

    bool _IsPartMsg(const QString &message);
    /*!
     * \brief Check if message means that someone was set as moderator in the chat.
     * \param message - message to check.
     * \return true, if it is a mode message.
     *
     * Checks if raw message mode message from twitch.tvt that notify about user that now have moderator role.
     */
    bool _IsModeMessage(const QString &message);
    /*!
     * \brief Check if message means that someone was downgraded from moderator in the chat.
     * \param message - message to check.
     * \return true, if it is a unmode message.
     *
     * Checks if raw message unmode message from twitch.tvt that notify about user that now do not have moderator role.
     */
    bool _IsUnmodeMessage(const QString &message);
    /*!
     * \brief Checks if current messages contains bits.
     * \param message - message to check.
     * \return true if it is a bits message.
     *
     * Checks if raw message is a chat message that contains bits.
     */
    bool _IsBits(const QString &message);
    /*!
     * \brief Check message if it is a channel message.
     * \param message - message to check.
     * \return true, if it is a channel message.
     *
     * Checks if raw message is a chat message that do not contains bits.
     */
    bool _IsPrivMsg(const QString &message);
    /*!
     * \brief Check if it is whisper message.
     * \param message - message to check.
     * \return true if it is a whisper message.
     *
     * Checks if raw message is whisper message from a user.
     */
    bool _IsWhisper(const QString &message);

    /*** Set functions ***/
    /*!
     * \brief Set time-stamp.
     *
     * Sets current time-stamp to this message.
     */
    void _SetTimeStamp();    

    /*** Chat message parameters ***/
    /*! Author of message*/
    QString _author;
    /*! Real user name */
    QString _realName;
    /*! Custom name color of author if it was set */
    QString _color;
    /*! Time-stamp of a moment when message was received */
    QString _timeStamp;
    /*! Message itself */
    QString _message;
    /*! Moderator flag */
    bool _isModerator;
    /*! Subscriber flag */
    bool _isSubscriber;
    /*! Type of the message */
    MessageType _type;
    /*! Bits */
    QString _bits;
    /*! UserID */
    int _userID;

    /*** Regular expressions ***/
    /*! Regular expression to catch PING messages from twitch. */
    static QRegularExpression _regExpPing;
    /*! Regular expression to catch PONG messages from twitch. */
    static QRegularExpression _regExpPong;
    /*! Regular expression to catch USERSTATE messages from twitch. */
    static QRegularExpression _regExpUserstate;
    /*! Regular expression to catch ROOMSTATE messages from twitch. */
    static QRegularExpression _regExpRoomState;
    /*! Regular expression to catch JOIN messages from twitch. */
    static QRegularExpression _regExpJoin;
    /*! Regular expression to catch PART messages from twitch. */
    static QRegularExpression _regExpPart;
    /*! Regular expression to catch MODE messages from twitch. */
    static QRegularExpression _regExpMode;
    /*! Regular expression to catch UNMODE messages from twitch. */
    static QRegularExpression _regExpUnmode;
    /*! Regular expression to catch BITS messages from twitch. */
    static QRegularExpression _regExpBits;
    /*! Regular expression to catch PRIVMSG messages from twitch. */
    static QRegularExpression _regExpPrivmsg;
    /*! Regular expression to catch WHISPER messages from twitch. */
    static QRegularExpression _regExpWhisper;
};

}
