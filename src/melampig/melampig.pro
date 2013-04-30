TEMPLATE = app

include( $$PROJECT_PATH/src/shared.pri )

DEFINES += IMPORT_MELAMPIG_ORM
DEFINES += IMPORT_MELAMPIG_UI

DESTDIR = $$PROJECT_PATH/bin
LIBS +=  -L$$PROJECT_PATH/lib
LIBS +=  -lorm
LIBS +=  -lui

TARGET =  melampig

INCLUDEPATH += $$PROJECT_PATH/src/orm
INCLUDEPATH += $$PROJECT_PATH/src/ui

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

