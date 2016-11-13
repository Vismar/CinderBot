#include "MainWindow.hpp"
#include "./Utils/Config/ConfigurationManager.hpp"
#include "./Utils/UserData/UserData.hpp"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString error = ConfigurationManager::Instance().Initialize();
    if (error.isEmpty())
    {
        UserData::Instance().Initialize();
        Ui::MainWindow w;
        w.show();

        return a.exec();
    }
    // If configuration manager do not initialized, just show an error message.
    else
    {
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();
    }

    return -1;
}
