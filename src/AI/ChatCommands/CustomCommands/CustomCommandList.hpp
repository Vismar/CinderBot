/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include "AI/ChatCommands/CommandList.hpp"
#include <QObject>

/*!
 * \brief Contains all chat command things.
 */
namespace Command
{
namespace CustomChatCmd
{
// TODO: Update comments in this file
/*!
 * Class CustomCommandList
 * Read and store all custom commands from database
 */
class CustomCommandList : public CommandList
{
    Q_OBJECT
public:
    CustomCommandList();

protected:
    ////////////////////////////////
    /// CommandList override
    void Initialize();
    void OnCfgParamChanged(Utils::Configuration::CfgParam cfgParam);

    /*!
     * Initialize command list with proper commands from database
     */
    virtual void _InitializeCommands();

    /*! Name of table which should be created to store commands */
    QString _commandTableName;
    /*! Name of table which should be created to store answers for commands */
    QString _commandAnswersTableName;

private slots:
    /*!
     * Update list of custom commands
     * \param tableName - name of the table which was updated
     */
    void _UpdateCommands(const QString &tableName);
};

}
}
