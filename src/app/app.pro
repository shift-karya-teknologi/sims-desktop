QT += core gui widgets sql printsupport
DESTDIR = $$PWD/../../dist/portable
TARGET = ShiftIMS
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    logindialog.cpp \
    changepassworddialog.cpp
HEADERS += \
        mainwindow.h \
    logindialog.h \
    changepassworddialog.h

FORMS += \
    logindialog.ui \
    changepassworddialog.ui
