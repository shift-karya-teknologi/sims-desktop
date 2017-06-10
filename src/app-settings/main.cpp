#include "mainwindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSettings settings("ShiftIMS.ini", QSettings::IniFormat);

    MainWindow mainWindow(&settings);
    mainWindow.show();

    return app.exec();
}
