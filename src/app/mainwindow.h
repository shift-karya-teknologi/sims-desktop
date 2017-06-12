#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void showLoginDialog();
    void showChangePasswordDialog();

private:
    void updateStatusBar();

    QLabel* usernameLabel;
};

#endif // MAINWINDOW_H
