#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QMenu;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void showLoginDialog();
    void showChangePasswordDialog();
    void showUserAndGroupManager();

private:
    void updateStatusBar();
    void updateMenusAndActions();

    QLabel* usernameLabel;

    QMenu* settingsMenu;
    QAction* manageUserAndGroupAction;
};

#endif // MAINWINDOW_H
