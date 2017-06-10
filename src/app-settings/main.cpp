#include "mainwindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSettings settings("sims.ini", QSettings::IniFormat);

    MainWindow mainWindow(&settings);
    mainWindow.show();

    return app.exec();
}
