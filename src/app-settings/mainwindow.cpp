#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasesettingswidget.h"

MainWindow::MainWindow(QSettings* settings)
    : ui(new Ui::MainWindow)
    , settings(settings)
    , databaseSettingsWidget(new DatabaseSettingsWidget(settings, this))
{
    ui->setupUi(this);

    ui->tabWidget->addTab(databaseSettingsWidget, "Basis &Data");
    connect(ui->okButton, SIGNAL(clicked(bool)), SLOT(save()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save()
{
    if (!databaseSettingsWidget->save())
        return;

    close();
}
