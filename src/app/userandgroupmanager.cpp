#include "userandgroupmanager.h"
#include "usermanager.h"
#include "usergroupmanager.h"

#include <QTabWidget>
#include <QBoxLayout>

UserAndGroupManager::UserAndGroupManager(QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{

    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->addTab(new UserManager(this), "&Pengguna");
    tabWidget->addTab(new UserGroupManager(this), "&Grup Pengguna");
    tabWidget->setCurrentIndex(0);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);

    setMinimumSize(600, 400);
    setWindowTitle("Pengguna dan Grup");
}
