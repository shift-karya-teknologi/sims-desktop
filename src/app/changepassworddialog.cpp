#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"

#include <QApplication>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::ChangePasswordDialog)
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(clicked(bool)), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}

void ChangePasswordDialog::accept()
{
    QString oldPassword = ui->password0Edit->text();
    QString newPassword = ui->password1Edit->text();
    QString newPasswordConfirmation = ui->password2Edit->text();

    uint currentUserId = qApp->property("CURRENT_USER").toMap().value("id").toUInt();

    if (newPassword != newPasswordConfirmation) {
        ui->password2Edit->setFocus();
        ui->password2Edit->selectAll();
        QMessageBox::warning(this, "Peringatan", "Kata sandi yang anda konfirmasi salah!");
        return;
    }


    QSqlQuery q;
    q.prepare("select password from users where id=?");
    q.bindValue(0, currentUserId);
    q.exec();
    if (!q.next()) {
        QMessageBox::critical(this, "Database Error", "Akun pengguna tidak ditemukan!");
        return;
    }

    QByteArray storedPassword = q.value("password").toByteArray();
    if (storedPassword != QCryptographicHash::hash(oldPassword.toUtf8(), QCryptographicHash::Sha1).toHex()) {
        ui->password0Edit->setFocus();
        ui->password0Edit->selectAll();
        QMessageBox::warning(this, "Peringatan", "Kata sandi salah!");
        return;
    }

    q.prepare("update users set password=? where id=?");
    q.bindValue(0, QCryptographicHash::hash(newPassword.toUtf8(), QCryptographicHash::Sha1).toHex());
    q.bindValue(1, currentUserId);
    q.exec();

    QDialog::accept();

    QMessageBox::information(this->parentWidget(), "Informasi", "Kata sandi telah diperbarui.");
}
