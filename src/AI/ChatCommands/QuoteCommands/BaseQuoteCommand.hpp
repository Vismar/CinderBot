#pragma once
#include "../ChatCommand.hpp"

namespace Command
{

/*!
 * Class BaseCovenantCommand
 * Base class for every quote command
 */
class BaseQuoteCommand : public ChatCommand
{
public:
    BaseQuoteCommand();
    /*!
     * Set reference to quotes
     * \param(IN) quotes - reference to list of quotes
     */
    void SetQuoteRef(QVector<QString>* quotes);

protected:
    /*!
     * Try to get number right after command.
     * \param(IN) command - command itself
     * \param(IN) message - message in which number should be found
     * \param(OUT) value - number that was found
     * \return true, if number was found
     */
    bool _GetNumberAfterCommand(const QString& command, const QString& message, QString& value);
    /*! Reference to list of quotes */
    QVector<QString>* _quotes;
};

}
