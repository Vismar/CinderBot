/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
**                                                                      **
** This program is free software: you can redistribute it and/or modify **
** it under the terms of the GNU General Public License as published by **
** the Free Software Foundation, either version 3 of the License, or    **
** (at your option) any later version.                                  **
**                                                                      **
** This program is distributed in the hope that it will be useful, but  **
** WITHOUT ANY WARRANTY; without even the implied warranty of           **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         **
** GNU General Public License for more details.                         **
**                                                                      **
** You should have received a copy of the GNU General Public License    **
** along with this program. If not, see <http://www.gnu.org/licenses/>. **
**************************************************************************/
#include "MainWindow.hpp"
#include "Utils/Config/ConfigurationManager.hpp"
#include "Utils/Database/DatabaseManager.hpp"
#include "Utils/Database/CustomCommandDBHelper.hpp"
#include "Utils/Database/QuoteDBHelper.hpp"
#include "Utils/Database/AnalyticsDBHelper.hpp"
#include "Utils/Database/UserDataDBHelper.hpp"
#include "Utils/UserData/RealTimeUserData.hpp"
#include "Twitch/KrakenClient.hpp"
#include "Utils/Logger.hpp"
#include <QApplication>
#include <QMessageBox>
#include <QtGlobal>
#include <QTime>

int main(int argc, char *argv[])
{
    LOG(Utils::LogInfo, "CinderBot started.");

    // Set seed for random generation
    qsrand(QTime::currentTime().second());
    int returningCode(0);    

    QApplication a(argc, argv);

    // Try to initialize database manager
    QString error = Utils::Database::DatabaseManager::Instance().Initialize();
    if (error != "OK")
    {
        // Log
        LOG(Utils::LogCritical, error);

        // Message box
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();

        // Return -1 as an error
        returningCode = -1;
    }
    else
    {
        // Log
        LOG(Utils::LogInfo, "DatabaseManager loaded.");
    }

    // Try to initialize custom command db helper
    error = Utils::Database::CustomCommandDBHelper::Instance().InititalizeTables();
    if (error != "OK")
    {
        // Log
        LOG(Utils::LogCritical, error);

        // Message box
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();

        // Return -1 as an error
        returningCode = -1;
    }
    else
    {
        // Log
        LOG(Utils::LogInfo, "CustomCommandDBHelper loaded.");
    }

    // Try to inititalize quote db helper
    error = Utils::Database::QuoteDBHelper::Instance().InitializeTables();
    if (error != "OK")
    {
        // Log
        LOG(Utils::LogCritical, error);

        // Message box
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();

        // Return -1 as an error
        returningCode = -1;
    }
    else
    {
        // Log
        LOG(Utils::LogInfo, "QuoteDBHelper loaded.");
    }
    
    // Try to initialize user data db helper
    error = Utils::Database::UserDataDBHelper::Instance().InitializeTables();
    if (error != "OK")
    {
        // Log
        LOG(Utils::LogCritical, error);

        // Message box
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();

        // Return -1 as an error
        returningCode = -1;
    }
    else
    {
        // Log
        LOG(Utils::LogInfo, "UserDataDBHelper loaded.");
    }

    // Try to initialize analytics db helper
    error = Utils::Database::AnalyticsDBHelper::Instance().InitializeTables();
    if (error != "OK")
    {
        // Log
        LOG(Utils::LogWarning, error);

        // Message box
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();
    }
    else
    {
        LOG(Utils::LogInfo, "AnalyticsDBHelper loaded.");
    }

    // Try to initialize configuration manager
    error = Utils::Configuration::ConfigurationManager::Instance().Initialize();
    if (error.isEmpty())
    {
        // Log
        LOG(Utils::LogInfo, "Configuration loaded.");

        // Inititalize kraken client
        error = Twitch::KrakenClient::Instance().Initialize();
        if (error != "OK")
        {
            // Log
            LOG(Utils::LogWarning, error);

            // Message box
            QMessageBox msgBox;
            msgBox.setText(error);
            msgBox.exec();
        }
        else
        {
            LOG(Utils::LogInfo, "KrakenClient loaded.");
        }
        
        // Initialize real time user data
        RealTimeUserData::Instance();
        // Create main window
        Ui::MainWindow mainWindow;
        mainWindow.show();

        // Log
        LOG(Utils::LogInfo, "App started.");        

        returningCode = a.exec();
    }
    // If configuration manager do not initialized, just show an error message.
    else
    {
        // Log
        LOG(Utils::LogCritical, error);

        // Message box
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();

        // Return -1 as an error
        returningCode = -1;
    }

    // Log
    LOG(Utils::LogWarning, "CinderBot closed.\n\n"
                           "///////////////////////////////////////////////////////////////////////////\n"
                           "////////////////////////////// END OF SESSION /////////////////////////////\n"
                           "///////////////////////////////////////////////////////////////////////////");

    // Flush all log messages
    LOG_FLUSH();

    return returningCode;
}
