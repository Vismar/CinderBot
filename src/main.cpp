#include "MainWindow.hpp"
#include "./Utils/Config/ConfigurationManager.hpp"
#include "./Utils/UserData/UserData.hpp"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (ConfigurationManager::Instance().Initialize())
    {
        UserData::Instance().Initialize();
        Ui::MainWindow w;
        w.show();

        return a.exec();
    }
    // If configuration manager do not initialized, just show a error message.
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Configuration manager can not be initialized! Check your config.cfg file!");
        msgBox.exec();
    }

    return -1;
}
