#pragma once
#include "../../Chat/ChatMessage.hpp"
#include <QString>
#include <QVector>
#include <QTime>
#include <QXmlStreamReader>

namespace Command
{

enum CmdSection
{
    Start,
    Name,
    Answer,
    Cooldown,
    ModOnly
};

/*!
 * Class ChatCommand
 * Provide functionality to read command data from passed xml stream reader(should be already initialized) and store found data.
 * Also handles getting random answer from all that was stored.
 */
class ChatCommand
{
public:
    /*!
     * Get section name
     * \param(IN) cmdSection - section type
     * \return section name
     */
    static QString GetSectionName(CmdSection cmdSection);
    /*!
     * \param(IN) xmlReader - xml stream reader
     * \return true if all needed params were found
     */
    bool Initialize(QXmlStreamReader& xmlReader);
    /*!
     * Check if chat command was inititalized
     * \return true if command was already initialized
     */
    bool IsInititalized() const;
    /*!
     * Try to execute command and return random answer
     * \param(IN) message - chat message
     * \return answer, if command was exectued. If command was not executed, string will be empty.
     */
    virtual QString GetRandomAnswer(const ChatMessage& message);

protected:
    /*!
     * Clears all saved data
     */
    void _Clear();
    /*!
     * Try to find "@" symbol and attach author name to it
     * \param(IN) answer - selected answer that will be parsed to add author name
     * \param(IN) author - author name
     */
    void _AddAuthorName(QString& answer, const QString& author);

    /*! Command name */
    QString _name;
    /*! List of answers */
    QVector<QString> _answers;
    /*! Time, when command was executed for a last time */
    QTime _lastTimeUsed;
    /*! Command cooldown */
    QTime _cooldown;
    /*! If command can be executed only be moderators */
    bool _moderatorOnly;
};

}
