#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint | Qt::Dialog)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(clicked(bool)), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));

    adjustSize();
    setFixedHeight(height());
    setFixedWidth(275);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::accept()
{
    const QString username = ui->usernameEdit->text().trimmed();
    const QString password = ui->passwordEdit->text();

    if (username.isEmpty()) {
        ui->usernameEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "ID pengguna harus diisi!");
        return;
    }

    QSqlQuery q;
    q.prepare("select id, username, fullname, is_active, is_admin, password"
              " from users"
              " where username=?");
    q.bindValue(0, username);
    q.exec();

    if (!q.next()) {
        ui->usernameEdit->setFocus();
        ui->usernameEdit->selectAll();
        QMessageBox::warning(this, "Peringatan", "Akun pengguna tidak ditemukan!");
        return;
    }

    if (!q.value("is_active").toBool()) {
        ui->usernameEdit->setFocus();
        ui->usernameEdit->selectAll();
        QMessageBox::warning(this, "Peringatan", "Akun pengguna tidak aktif!");
        return;
    }

    QByteArray storedPassword = q.value("password").toByteArray();
    if (storedPassword != QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1).toHex()) {
        ui->passwordEdit->setFocus();
        ui->passwordEdit->selectAll();
        QMessageBox::warning(this, "Peringatan", "Kata sandi yang anda masukkan salah!");
        return;
    }

    // Set current user
    QVariantMap currentUser;
    currentUser.insert("id", q.value("id").toUInt());
    currentUser.insert("username", q.value("username").toString());
    currentUser.insert("fullname", q.value("fullname").toString());
    currentUser.insert("is_admin", q.value("is_admin").toBool());
    qApp->setProperty("CURRENT_USER", currentUser);

    QDialog::accept();
}
