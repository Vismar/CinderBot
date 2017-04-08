#pragma once
#include "BaseQuoteCommand.hpp"

namespace Command
{

/*!
 * Class AddQuoteCommand
 * Add quote command
 */
class AddQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    AddQuoteCommand();

    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
};

}
