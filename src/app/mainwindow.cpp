#include "mainwindow.h"
#include "logindialog.h"

#include "changepassworddialog.h"
#include <QApplication>
#include <QStatusBar>
#include <QLabel>

MainWindow::MainWindow()
{
    setMinimumSize(640, 480);

    // setup status bar
    QStatusBar* statusBar = this->statusBar();
    usernameLabel = new QLabel(statusBar);
    statusBar->addPermanentWidget(usernameLabel);
    connect(usernameLabel, SIGNAL(linkActivated(QString)), SLOT(showChangePasswordDialog()));
}

void MainWindow::showLoginDialog()
{
    if (qApp->property("CURRENT_USER").isValid()) {
        // Clear auth
        qApp->setProperty("CURRENT_USER", QVariantMap());
    }

    hide();

    LoginDialog dialog;
    if (!dialog.exec()) {
        close();
        return;
    }

    updateStatusBar();
    show();
}

void MainWindow::updateStatusBar()
{
    QVariantMap currentUser = qApp->property("CURRENT_USER").toMap();
    usernameLabel->setText(QString("<a href=\"#\">%1</a>").arg(currentUser.value("username").toString()));
}

void MainWindow::showChangePasswordDialog()
{
    ChangePasswordDialog dialog;
    dialog.exec();
}
