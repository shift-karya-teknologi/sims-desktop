#ifndef USERGROUPEDITOR_H
#define USERGROUPEDITOR_H

#include <QDialog>

namespace Ui {
class UserGroupEditor;
}

class UserGroupEditor : public QDialog
{
    Q_OBJECT

public:
    explicit UserGroupEditor(uint id, QWidget *parent);
    ~UserGroupEditor();

public slots:
    void accept();

private slots:
    void showMemberContextMenu(const QPoint& pos);
    void removeSelectedMember();

private:
    class MemberModel;



    Ui::UserGroupEditor *ui;
    uint id;

    MemberModel* memberModel;
};

#endif // USERGROUPEDITOR_H
