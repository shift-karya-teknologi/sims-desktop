QT = core gui widgets sql
TARGET = ShiftIMS-Settings
DESTDIR = $$PWD/../../dist/portable
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    databasesettingswidget.cpp

HEADERS += \
    mainwindow.h \
    databasesettingswidget.h

FORMS += \
    mainwindow.ui \
    databasesettingswidget.ui
