#include "MainWindow.hpp"
#include "./Utils/Config/ConfigurationManager.hpp"
#include "./Utils/UserData/UserData.hpp"
#include <QApplication>
#include <QMessageBox>
#include <QtGlobal>
#include <QTime>

int main(int argc, char *argv[])
{
    // Set seed for random generation
    qsrand(QTime::currentTime().second());
    int returningCode(0);
    QApplication a(argc, argv);
    // Try to initialize configuration manager
    QString error = ConfigurationManager::Instance().Initialize();
    if (error.isEmpty())
    {
        // Initialize user data
        UserData::Instance().Initialize();
        // Create main window
        Ui::MainWindow w;
        w.show();

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
