/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QStringList>

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
     * Desctructor
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
     * Add new answer to list
     * \param answer - new message in list
     */
    void AddAnswer(const QString &answer);
    /*!
     * Return reference to list of answers
     * \return const reference to list of answers
     */
    const QStringList& GetAnswers() const;

private:
    /*! Type of answer */
    ChatAnswerType _type;
    /*! List of answers */
    QStringList _answers;
};
