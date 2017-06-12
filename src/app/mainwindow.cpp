#include "mainwindow.h"
#include "logindialog.h"
#include "changepassworddialog.h"
#include "userandgroupmanager.h"

#include <QApplication>
#include <QStatusBar>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>

MainWindow::MainWindow()
{
    setMinimumSize(640, 480);

    // setup menus
    QMenuBar* menuBar = this->menuBar();
    settingsMenu = menuBar->addMenu("Pengaturan");
    manageUserAndGroupAction = settingsMenu->addAction("&Pengguna dan Grup");
    connect(manageUserAndGroupAction, SIGNAL(triggered(bool)), SLOT(showUserAndGroupManager()));

    // setup status bar
    QStatusBar* statusBar = this->statusBar();
    usernameLabel = new QLabel(statusBar);
    statusBar->addPermanentWidget(usernameLabel);
    connect(usernameLabel, SIGNAL(linkActivated(QString)), SLOT(showChangePasswordDialog()));
}

void MainWindow::updateMenusAndActions()
{
    QVariantMap currentUser = qApp->property("CURRENT_USER").toMap();
    bool isAdmin = currentUser.value("is_admin").toBool();
    manageUserAndGroupAction->setEnabled(isAdmin);
}

void MainWindow::showLoginDialog()
{
    hide();

    LoginDialog dialog;
    if (!dialog.exec()) {
        close();
        return;
    }

    updateStatusBar();
    updateMenusAndActions();

    show();
}

void MainWindow::updateStatusBar()
{
    QVariantMap currentUser = qApp->property("CURRENT_USER").toMap();
    QString fullName = currentUser.value("fullname").toString();
    QString username = currentUser.value("username").toString();
    QString userinfo = fullName.isEmpty() ? username : fullName;
    usernameLabel->setText(QString("<a href=\"#\">%1</a>").arg(userinfo));
}

void MainWindow::showChangePasswordDialog()
{
    ChangePasswordDialog dialog;
    dialog.exec();
}

void MainWindow::showUserAndGroupManager()
{
    UserAndGroupManager manager;
    manager.exec();
}
