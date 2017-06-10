#include "databasesettingswidget.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QSettings>

DatabaseSettingsWidget::DatabaseSettingsWidget(QSettings* settings, QWidget *parent)
    : QWidget(parent)
    , _settings(settings)
    , _isSqlite(false)
    , _isMySql(false)
{
    setupUi(this);

    for (QString driverName: QSqlDatabase::drivers()) {
        // Driver yang bisa digunakan
        if (driverName == "QSQLITE" || driverName == "QMYSQL")
            driverComboBox->addItem(driverName);
    }
    driverComboBox->setCurrentIndex(-1);

    connect(driverComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateUi()));
    connect(testButton, SIGNAL(clicked(bool)), SLOT(testConnection()));

    _settings->beginGroup("Database");
    driverComboBox->setCurrentText(_settings->value("Driver").toString());
    hostEdit->setText(_settings->value("Host").toString());
    portSpinBox->setValue(_settings->value("Port").toInt());
    schemaEdit->setText(_settings->value("Schema").toString());
    usernameEdit->setText(_settings->value("Username").toString());
    passwordEdit->setText(_settings->value("Password").toString());
    _settings->endGroup();

    updateUi();
}

void DatabaseSettingsWidget::testConnection()
{
    if (!validate())
        return;

    {
        QSqlDatabase db = QSqlDatabase::addDatabase(driverComboBox->currentText());
        db.setHostName(hostEdit->text());
        db.setPort(portSpinBox->value());
        db.setDatabaseName(schemaEdit->text());
        db.setUserName(usernameEdit->text());
        db.setPassword(passwordEdit->text());

        if (!db.open()) {
            QMessageBox mb(this);
            mb.setWindowTitle("Kesalahan Koneksi Basis Data");
            mb.setText("Tidak dapat terhubung dengan basis data.");
            mb.setDetailedText(db.lastError().text());
            mb.exec();
        }

        // TODO: Validasi versi database
    }

    QSqlDatabase::database(QSqlDatabase::defaultConnection).close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    QMessageBox::information(this, "Koneksi Berhasil", "Koneksi basis data valid.");
}

void DatabaseSettingsWidget::updateUi()
{
    const QString driver = driverComboBox->currentText();

    _isSqlite = driver == "QSQLITE";
    _isMySql = driver == "QMYSQL" || driver == "QMYSQL3";

    hostLabel->setEnabled(_isMySql);
    hostEdit->setEnabled(_isMySql);
    usernameEdit->setEnabled(_isMySql);
    usernameLabel->setEnabled(_isMySql);
    passwordEdit->setEnabled(_isMySql);
    passwordLabel->setEnabled(_isMySql);
    portSpinBox->setEnabled(_isMySql);
    portLabel->setEnabled(_isMySql);

    if (_isSqlite) {
        schemaEdit->setFocus();
        schemaEdit->selectAll();
        schemaLabel->setText("Lokasi &File");
    }
    else if (_isMySql) {
        hostEdit->setFocus();
        hostEdit->selectAll();
        schemaLabel->setText("Nama &Skema");
    }
}

bool DatabaseSettingsWidget::save()
{
    if (!validate())
        return false;

    _settings->beginGroup("Database");
    _settings->setValue("Driver", driverComboBox->currentText());
    _settings->setValue("Host", hostEdit->text());
    _settings->setValue("Port", portSpinBox->value());
    _settings->setValue("Schema", schemaEdit->text());
    _settings->setValue("Username", usernameEdit->text());
    _settings->setValue("Password", passwordEdit->text());
    _settings->endGroup();

    return true;
}

bool DatabaseSettingsWidget::validate()
{
    if (driverComboBox->currentIndex() == -1) {
        driverComboBox->setFocus();
        QMessageBox::warning(this, "Peringatan", "Silahkan tentukan driver basis data!");
        driverComboBox->showPopup();
        return false;
    }

    if (_isMySql && hostEdit->text().isEmpty()) {
        hostEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Silahkan tentukan nama host!");
        return false;
    }

    if (_isMySql && schemaEdit->text().isEmpty()) {
        schemaEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Silahkan tentukan nama skema!");
        return false;
    }
    else if (_isSqlite && schemaEdit->text().isEmpty()) {
        schemaEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Silahkan tentukan lokasi berkas basis data!");
        return false;
    }

    if (_isMySql && usernameEdit->text().isEmpty()) {
        usernameEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Silahkan tentukan nama pengguna!");
        return false;
    }

    return true;
}
