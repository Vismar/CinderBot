#include "MainWindow.hpp"
#include "./Utils/Config/ConfigurationManager.hpp"
#include "./Utils/UserData/UserData.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    if (ConfigurationManager::Instance().Initialize())
    {
        UserData::Instance().Initialize();
        QApplication a(argc, argv);
        Ui::MainWindow w;
        w.show();

        return a.exec();
    }
    else
    {
        // Should be replaced by error window!
        qDebug() << "Configuration manager can not be initialized!\n";
    }

    return -1;
}
