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
};

}
