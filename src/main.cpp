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
#include "Utils/DatabaseManager.hpp"
#include "Utils/UserData/UserData.hpp"
#include "Utils/UserData/RealTimeUserData.hpp"
#include <QApplication>
#include <QMessageBox>
#include <QtGlobal>
#include <QTime>

int main(int argc, char *argv[])
{
    // Set seed for random generation
    qsrand(QTime::currentTime().second());
    int returningCode(0);
    QString error;

    QApplication a(argc, argv);

    // Try to initialize database manager
    error = DatabaseManager::Instance().Initialize();
    if (error != "OK")
    {
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();
        // Return -1 as an error
        returningCode = -1;
    }

    // Try to initialize configuration manager
    error = ConfigurationManager::Instance().Initialize();
    if (error.isEmpty())
    {
        // Initialize user data
        UserData::Instance().Initialize();
        // Initialize real time user data
        RealTimeUserData::Instance();
        // Create main window
        Ui::MainWindow mainWindow;
        mainWindow.show();

        returningCode = a.exec();
    }
    // If configuration manager do not initialized, just show an error message.
    else
    {
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();
        // Return -1 as an error
        returningCode = -1;
    }

    return returningCode;
}
