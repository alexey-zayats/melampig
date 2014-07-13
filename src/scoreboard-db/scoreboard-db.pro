TEMPLATE = app
QT       += core gui svg phonon sql

DESTDIR = $$PROJECT_PATH/bin

HEADERS += \
    fightcontrol-db.h \
    fightdisplay-db.h \
    fighttable-db.h \
    mainwindow-db.h \
    settingsform-db.h \
    winnerdialog-db.h \
    fightdisplayfila-db.h

FORMS += \
    fightcontrol-db.ui \
    fightdisplay-db.ui \
    mainwindow-db.ui \
    settingsform-db.ui \
    splash-db.ui \
    winnerdialog-db.ui \
    fightdisplayfila-db.ui

SOURCES += \
    scoreboard-db.cpp \
    mainwindow-db.cpp \
    settingsform-db.cpp \
    winnerdialog-db.cpp \
    fightcontrol-db.cpp \
    fightdisplay-db.cpp \
    fighttable-db.cpp \
    fightdisplayfila-db.cpp

win32 {
    PGDIR        = "C:\\Program Files (x86)\\PostgreSQL\\9.3"
    PATH        += "$$PGDIR\\bin"
    INCLUDEPATH += "$$PGDIR\\include"
    LIBS        += "-L$$PGDIR\\lib" -llibpq
}

linux-g++  {
    INCLUDEPATH += /usr/include/postgresql
    LIBS        += -L/usr/lib -lpq
}

LIBS +=  -L$$PROJECT_PATH/lib
LIBS +=  -lorm
LIBS +=  -lui

TARGET = scoreboard-db

DEFINES += IMPORT_MELAMPIG_ORM
DEFINES += IMPORT_MELAMPIG_UI

INCLUDEPATH += $$PROJECT_PATH/src/orm
INCLUDEPATH += $$PROJECT_PATH/src/ui

RESOURCES = $$PROJECT_PATH/melampig.qrc

unix:QMAKE_RPATHDIR += $$PROJECT_PATH/lib
