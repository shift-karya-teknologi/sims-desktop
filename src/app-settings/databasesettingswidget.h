#ifndef DATABASESETTINGSWIDGET_H
#define DATABASESETTINGSWIDGET_H

#include <QWidget>
#include "ui_databasesettingswidget.h"

class QSettings;

class DatabaseSettingsWidget
    : public QWidget
    , public Ui_DatabaseSettingsWidget
{
    Q_OBJECT

public:
    explicit DatabaseSettingsWidget(QSettings* settings, QWidget *parent);

    bool save();
    bool validate();

public slots:
    void testConnection();
    void updateUi();

private:
    QSettings* _settings;
    bool _isSqlite;
    bool _isMySql;
};

#endif // DATABASESETTINGSWIDGET_H
