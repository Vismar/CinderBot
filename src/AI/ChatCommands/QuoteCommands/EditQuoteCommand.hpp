#pragma once
#include "BaseQuoteCommand.hpp"

namespace Command
{

/*!
 * Class EditQuoteCommand
 * Edit quote command
 */
class EditQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    EditQuoteCommand();

    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
};

}
