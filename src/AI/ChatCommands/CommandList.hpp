/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/BaseChatCommand.hpp"
#include "Chat/ChatAnswer.hpp"
#include "Utils/Config/ConfigurationParameters.hpp"
#include <QVector>
#include <QObject>

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{

/*!
 * \brief Simple and base command list.
 *
 * Command list that stores multiple chat commands.
 */
class CommandList : public QObject
{
    Q_OBJECT
public:
    CommandList();
    virtual ~CommandList();
    /*!
     * \brief Try to get answer from stored list of commands.
     * \param message - chat message.
     * \param answer - returning answer.
     * \return true if one of commands from command list was executed.
     *
     * Check all commands to find one which should be executed. Can be overridden if it is needed.
     */
    virtual bool TryExecute(const ChatMessage &message, ChatAnswer &answer);
    /*!
     * \brief Initialize command list.
     *
     * Initialize needed parameters and data. Should be implemented by derived class.
     */
    virtual void Initialize() = 0;

protected slots:
    /*!
     * \brief Catch changes in configuration manager.
     * \param cfgParam - configuration parameter.
     *
     * Pure virtual function which should be implemented
     * by derived class to handle changes in configuration manager.
     */
    virtual void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam) = 0;

protected:
    /*! List of commands */
    QVector<BaseChatCommand*> _commands;
    /*! On/off flag */
    bool _isTurnedOn;
};

}
