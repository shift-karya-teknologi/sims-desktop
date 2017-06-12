#include "usergroupeditor.h"
#include "ui_usergroupeditor.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QStringListModel>
#include <QDebug>
#include <QSqlError>
#include <QMenu>

// TODO: Implementasikan delagate untuk editor member
// TODO: Implementasikan editor hak akses

class UserGroupEditor::MemberModel : public QAbstractTableModel
{
public:
    struct Item {
        uint id;
        QString username;
        QString fullname;
        Item() : id(0){}
    };
    typedef QList<Item> Items;
    Items items;

    MemberModel(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        if (index.column() == 0)
            flags |= Qt::ItemIsEditable;
        return flags;
    }

    int rowCount(const QModelIndex & = QModelIndex()) const
    {
        return items.size() + 1;
    }

    int columnCount(const QModelIndex & = QModelIndex()) const
    {
        return 2;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if (index.row() >= 0 && index.row() < items.size()) {
            Item item = items.at(index.row());
            if (role == Qt::EditRole || role == Qt::DisplayRole) {
                switch (index.column()) {
                case 0: return item.username;
                case 1: return item.fullname;
                }
            }
        }

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole) {
            if (orientation == Qt::Horizontal) {
                switch (section) {
                case 0: return "ID Pengguna";
                case 1: return "Nama Lengkap";
                }
            }
            else {
                if (section == items.size()) {
                    return "*";
                }
            }
        }
        else if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        }

        return QVariant();
    }

    void setItems(const Items& items)
    {
        beginResetModel();
        this->items = items;
        endResetModel();
    }

    bool removeRows(int row, int count, const QModelIndex& = QModelIndex())
    {
        Q_UNUSED(count);
        beginRemoveRows(QModelIndex(), row, row);
        items.removeAt(row);
        endRemoveRows();
        return true;
    }
};

UserGroupEditor::UserGroupEditor(uint id, QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint | Qt::Dialog)
    , ui(new Ui::UserGroupEditor)
    , id(id)
    , memberModel(new MemberModel(this))
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(clicked(bool)), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));

    ui->memberView->setModel(memberModel);
    ui->memberView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->memberView->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(showMemberContextMenu(QPoint)));

    if (id) {
        setWindowTitle(QString("Perbarui Grup Pengguna #%1").arg(QString::number(id)));

        QSqlQuery q;
        q.prepare("select * from user_groups where id=?");
        q.bindValue(0, id);
        q.exec();
        q.next();

        ui->nameEdit->setText(q.value("name").toString());
        ui->descriptionEdit->setPlainText(q.value("description").toString());

        MemberModel::Items users;

        QSqlQuery q2;
        q2.prepare("select u.id `id`, u.username `username`, u.fullname `fullname`"
                   " from user_group_members m"
                   " inner join users u on u.id = m.user_id"
                   " where m.group_id=?"
                   " order by u.username asc");
        q2.bindValue(0, id);
        q2.exec();
        while (q2.next()) {
            MemberModel::Item user;
            user.id = q2.value("id").toUInt();
            user.username = q2.value("username").toString();
            user.fullname = q2.value("fullname").toString();
            users << user;
        }

        memberModel->setItems(users);

        // TODO: muat hak akses
    }
    else {
        setWindowTitle("Buat Grup Pengguna");
    }

    ui->nameEdit->setFocus();
}

UserGroupEditor::~UserGroupEditor()
{
    delete ui;
}

void UserGroupEditor::accept()
{
    bool isNew = id == 0;

    QString name = ui->nameEdit->text().trimmed();
    QString description = ui->descriptionEdit->toPlainText();
    description.truncate(255);

    if (name.isEmpty()) {
        ui->nameEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Nama grup harus diisi!");
        return;
    }

    // Periksa duplikat
    QSqlQuery q;
    if (isNew) {
        q.prepare("select count(0) from user_groups where name=:name");
    }
    else {
        q.prepare("select count(0) from user_groups where name=:name and id<>:id");
        q.bindValue(":id", id);
    }
    q.bindValue(":name", name);
    q.exec();
    q.next();
    if (q.value(0).toInt() > 0) {
        ui->nameEdit->setFocus();
        ui->nameEdit->selectAll();
        QMessageBox::warning(this, "Peringatan", "Nama grup sudah digunakan!");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();

    db.transaction();

    // Simpan data grup
    if (isNew) {
        q.prepare("insert into user_groups (name, description) values (:name,:description)");
    }
    else {
        q.prepare("update user_groups set name=:name, description=:description where id=:id");
        q.bindValue(":id", id);
    }
    q.bindValue(":name", name);
    q.bindValue(":description", description);
    q.exec();


    if (isNew) {
        id = q.lastInsertId().toUInt();
    }

    // update member

    if (!isNew) {
        q.prepare("delete from user_group_members where group_id=?");
        q.bindValue(0, id);
        q.exec();
    }

    for (MemberModel::Item user : memberModel->items) {
        q.prepare("insert into user_group_members(group_id,user_id)"
                  " values(:group_id,:user_id)");
        q.bindValue(":group_id", id);
        q.bindValue(":user_id", user.id);
        q.exec();
    }

    // TODO: Update hak akses

    db.commit();

    QDialog::accept();
}

void UserGroupEditor::showMemberContextMenu(const QPoint& pos)
{
    int row = ui->memberView->verticalHeader()->logicalIndexAt(pos);
    if (row == memberModel->items.size())
        return;

    ui->memberView->selectRow(row);

    QMenu menu;
    menu.addAction("Hapus", this, SLOT(removeSelectedMember()));
    menu.exec(ui->memberView->verticalHeader()->mapToGlobal(pos));
}

void UserGroupEditor::removeSelectedMember()
{
    int row = ui->memberView->currentIndex().row();
    MemberModel::Item item = memberModel->items.at(row);

    if (QMessageBox::question(this, "Konfirmasi", QString("Hapus %1 dari daftar anggota grup?").arg(item.username),
                              "&Ya", "&Tidak"))
        return;

    memberModel->removeRow(row);
}
