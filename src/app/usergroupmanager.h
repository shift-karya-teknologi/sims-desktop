#ifndef USERGROUPMANAGER_H
#define USERGROUPMANAGER_H

#include <QWidget>

class QTableView;
class QLabel;

class UserGroupManager : public QWidget
{
    Q_OBJECT

public:
    explicit UserGroupManager(QWidget *parent);

public slots:
    void refresh();
    void add();
    void edit();
    void remove();
    void updateActions();

private:
    class Model;
    class ProxyModel;

    Model* model;
    ProxyModel* proxyModel;
    QTableView* view;
    QLabel* infoLabel;
    QAction* refreshAction;
    QAction* addAction;
    QAction* removeAction;
};

#endif // USERGROUPMANAGER_H
