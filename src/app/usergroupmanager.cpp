#include "usergroupmanager.h"
#include "usergroupeditor.h"

#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QToolBar>
#include <QTableView>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QBoxLayout>
#include <QMessageBox>

class UserGroupManager::Model : public QAbstractTableModel
{
public:
    struct Item
    {
        uint id;
        QString name;
        QString description;
        Item() : id(0) {}
    };

    typedef QList<Item> Items;
    Items items;

    Model(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    int rowCount(const QModelIndex& = QModelIndex()) const
    {
        return items.size();
    }

    int columnCount(const QModelIndex& = QModelIndex()) const
    {
        return 3;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        Item item = items.at(index.row());
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            switch (index.column()) {
            case 0: return item.id;
            case 1: return item.name;
            case 2: return item.description.replace("\n", " ");
            }
        }
        else if (role == Qt::TextAlignmentRole) {
            switch (index.column()) {
            case 0: return Qt::AlignVCenter ^ Qt::AlignRight;
            case 1: return Qt::AlignVCenter ^ Qt::AlignLeft;
            case 2: return Qt::AlignVCenter ^ Qt::AlignLeft;
            }
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            switch (section) {
            case 0: return "ID";
            case 1: return "Nama Grup";
            case 2: return "Deskripsi";
            }
        }
        return QVariant();
    }

    void refresh()
    {
        QSqlQuery q;
        q.prepare("select * from user_groups");
        q.exec();

        Items tmpItems;
        while (q.next()) {
            Item item;
            item.id = q.value("id").toUInt();
            item.name = q.value("name").toString();
            item.description = q.value("description").toString();
            tmpItems.append(item);
        }

        beginResetModel();
        items = tmpItems;
        endResetModel();
    }
};

class UserGroupManager::ProxyModel : public QSortFilterProxyModel
{
public:
    ProxyModel(QObject* parent)
        : QSortFilterProxyModel(parent)
    {
        setSortCaseSensitivity(Qt::CaseInsensitive);
        setSortRole(Qt::EditRole);
    }
};

UserGroupManager::UserGroupManager(QWidget *parent)
    : QWidget(parent)
{
    model = new Model(this);

    proxyModel = new ProxyModel(this);
    proxyModel->setSourceModel(model);

    QToolBar* toolBar = new QToolBar(this);

    refreshAction = toolBar->addAction("&Muat Ulang");
    refreshAction->setShortcut(QKeySequence("F5"));
    connect(refreshAction, SIGNAL(triggered(bool)), SLOT(refresh()));

    addAction = toolBar->addAction("&Buat");
    addAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(addAction, SIGNAL(triggered(bool)), SLOT(add()));

    removeAction = toolBar->addAction("&Hapus");
    removeAction->setShortcut(QKeySequence("Del"));
    connect(removeAction, SIGNAL(triggered(bool)), SLOT(remove()));

    view = new QTableView(this);
    view->setAlternatingRowColors(true);
    view->setTabKeyNavigation(false);
    view->setSortingEnabled(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->horizontalHeader()->setHighlightSections(false);
    view->verticalHeader()->setVisible(false);
    view->verticalHeader()->setMinimumSectionSize(20);
    view->verticalHeader()->setMaximumSectionSize(20);
    view->setModel(proxyModel);
    connect(view, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(updateActions()));

    infoLabel = new QLabel(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(view);
    mainLayout->addWidget(infoLabel);

    refresh();
}

void UserGroupManager::refresh()
{
    model->refresh();

    view->resizeColumnsToContents();
    view->resizeRowsToContents();
    view->horizontalHeader()->setStretchLastSection(true);

    infoLabel->setText(QString("Menampilkan %1 grup pengguna").arg(QString::number(proxyModel->rowCount())));

    updateActions();
}

void UserGroupManager::add()
{
    UserGroupEditor editor(0, this);
    if (!editor.exec())
        return;

    refresh();
}

void UserGroupManager::edit()
{
    QModelIndex index = view->currentIndex();
    if (!index.isValid())
        return;

    Model::Item item = model->items.at(proxyModel->mapToSource(index).row());

    UserGroupEditor editor(item.id, this);
    if (!editor.exec())
        return;

    refresh();
}

void UserGroupManager::remove()
{
    QModelIndex index = view->currentIndex();
    if (!index.isValid())
        return;

    Model::Item item = model->items.at(proxyModel->mapToSource(index).row());

    if (QMessageBox::question(this, "Konfirmasi",
                              QString("Hapus grup <b>#%1 %2</b>?").arg(QString::number(item.id), item.name),
                              "&Ya", "&Tidak"))
        return;

    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery q;
    q.prepare("delete from user_group_members where group_id=?");
    q.bindValue(0, item.id);
    q.exec();

    q.prepare("delete from user_groups where id=?");
    q.bindValue(0, item.id);
    q.exec();

    db.commit();

    refresh();
}

void UserGroupManager::updateActions()
{
    removeAction->setEnabled(view->selectionModel()->hasSelection());
}
