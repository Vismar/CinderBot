/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QStringList>

/*!
 * \brief Contains all things for chat.
 */
namespace Chat
{

enum ChatAnswerType
{
    Twitch_Whisper,
    Twitch_Chat
};

/*!
 * class ChatAnswer
 * This class store type of answer and the content which will be sent in required way.
 */
class ChatAnswer
{
public:
    /*!
     * Constructor
     */
    ChatAnswer();
    /*!
     * Destructor
     */
    ~ChatAnswer();

    /*!
     * Return type of answer
     * \return type of answer, it will be used to know how to send message
     */
    ChatAnswerType GetType() const;
    /*!
     * Set specified type to chat answer
     */
    void SetType(ChatAnswerType type);
    /*!
     * Return real name of user for whom answer should be sent if it was a whisper
     * \return string with real user name
     */
    const QString& GetRealName() const;
    /*!
     * Set real name of user for whom answer should be sent if it was a whisper
     * \param realName - real name which will be set
     */
    void SetRealName(const QString &realName);
    /*!
     * Add new answer to list
     * \param answer - new message in list
     */
    void AddAnswer(const QString &answer);
    /*!
     * Return reference to list of answers
     * \return const reference to list of answers
     */
    QStringList& GetAnswers();

private:
    /*! Type of answer */
    ChatAnswerType _type;
    /*! Real name of user for whom answer should be sent if it was a whisper */
    QString _realName;
    /*! List of answers */
    QStringList _answers;
};

}
