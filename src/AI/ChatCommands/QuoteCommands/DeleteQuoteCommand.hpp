#pragma once
#include "BaseQuoteCommand.hpp"

namespace Command
{

/*!
 * Class DeleteQuoteCommand
 * Delete quote command
 */
class DeleteQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    DeleteQuoteCommand();

    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;

private:
    /*!
     * Update quote numbers
     * \param(IN) quoteNumber - number of deleted quote
     */
    void _RefreshQuoteNumbers(int quoteNumber);
};

}
