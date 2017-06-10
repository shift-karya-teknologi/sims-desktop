#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QSettings;

namespace Ui {
class MainWindow;
}

class DatabaseSettingsWidget;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QSettings* settings);
    ~MainWindow();

public slots:
    void save();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    DatabaseSettingsWidget* databaseSettingsWidget;
};

#endif // MAINWINDOW_H
