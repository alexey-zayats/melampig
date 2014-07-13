TEMPLATE = app

include( ../shared.pri )
RESOURCES = ../../melampig.qrc

DEFINES += IMPORT_MELAMPIG_ORM
DEFINES += IMPORT_MELAMPIG_UI

DESTDIR = ../../bin
LIBS +=  -L../../lib
LIBS +=  -lorm
LIBS +=  -lui

TARGET =  melampig

INCLUDEPATH += ../orm
INCLUDEPATH += ../ui

TRANSLATIONS =  melampig_ru.ts \
    melampig_en.ts

HEADERS += \
    mainwindow.h \
    pluginbase.h

SOURCES += \
    mainwindow.cpp \
    melampig.cpp \
    pluginbase.cpp

FORMS += mainwindow.ui

