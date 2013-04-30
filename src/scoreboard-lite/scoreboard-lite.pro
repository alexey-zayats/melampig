TEMPLATE = app

QT       += core gui svg phonon

DESTDIR = $$PROJECT_PATH/bin

SOURCES += \
    scoreboard-lite.cpp\
    mainwindow-s.cpp \
    fightdisplay.cpp \
    fightcontrol.cpp \
    settingsform.cpp

HEADERS  += \
    mainwindow-s.h \
    fightdisplay.h \
    fightcontrol.h \
    settingsform.h

FORMS    += \
    mainwindow-s.ui \
    fightdisplay.ui \
    fightcontrol.ui \
    splash.ui \
    settingsform.ui

LIBS +=  -L$$PROJECT_PATH/lib

TARGET = scoreboard-lite

RESOURCES = $$PROJECT_PATH/melampig.qrc
