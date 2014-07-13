TEMPLATE = app

QT       += core gui svg phonon

DESTDIR = ../../bin

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

LIBS +=  -L../../lib

TARGET = scoreboard-lite

RESOURCES = ../../melampig.qrc
