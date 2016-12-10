#pragma once
#include "BaseQuoteCommand.hpp"

namespace Command
{

/*!
 * Class MainQuoteCommand
 * Main quote command
 */
class MainQuoteCommand : public BaseQuoteCommand
{
public:
    /*! Constructor */
    MainQuoteCommand();

    ////////////////////////////////
    /// ChatCommand overrides
    QString GetRandomAnswer(const ChatMessage& message) override;
};

}
