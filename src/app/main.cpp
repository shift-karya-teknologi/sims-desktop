#include "mainwindow.h"
#include <QApplication>
#include <QTimer>
#include <QSqlDatabase>
#include <QSettings>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Shift IMS");

    {
        QSettings settings("ShiftIMS.ini", QSettings::IniFormat);
        settings.beginGroup("Database");
        QSqlDatabase db = QSqlDatabase::addDatabase(settings.value("Driver").toString());
        db.setHostName(settings.value("Host").toString());
        db.setPort(settings.value("port").toInt());
        db.setDatabaseName(settings.value("Schema").toString());
        db.setUserName(settings.value("username").toString());
        db.setPassword(settings.value("password").toString());
        settings.endGroup();
        if (!db.open()) {
            QMessageBox::critical(0, "Kesalahan Basis Data", "Tidak dapat terhubung ke pusat basis data!");
            qCritical() << db.lastError().text();
            return 1;
        }
    }

    MainWindow mainWindow;

    QTimer::singleShot(0, &mainWindow, SLOT(showLoginDialog()));

    int exitCode = app.exec();

    {
        QSqlDatabase::database(QSqlDatabase::defaultConnection, false).close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    return exitCode;
}
