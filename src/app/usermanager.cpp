#include "usermanager.h"

#include <QToolBar>
#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QBoxLayout>

UserManager::UserManager(QWidget *parent)
    : QWidget(parent)
{
    QToolBar* toolBar = new QToolBar(this);

    view = new QTableView(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(view);
}
