#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>

class QTableView;

class UserManager : public QWidget
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent);

private:
    QTableView* view;
};

#endif // USERMANAGER_H
