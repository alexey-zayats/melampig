
TEMPLATE = lib

include ( $$PROJECT_PATH/src/shared.pri )

TRANSLATIONS =  reference_ru.ts \
		reference_en.ts
CONFIG += dll

DEFINES += IMPORT_MELAMPIG_ORM
DEFINES += IMPORT_MELAMPIG_UI

INCLUDEPATH += $$PROJECT_PATH/src/melampig
INCLUDEPATH += $$PROJECT_PATH/src/orm
INCLUDEPATH += $$PROJECT_PATH/src/ui

DESTDIR = $$PROJECT_PATH/lib
LIBS +=  -L$$PROJECT_PATH/lib

LIBS += -lorm
LIBS += -lui

TARGET = reference

SOURCES += \
    reference.cpp \
    referencestack.cpp

HEADERS += \
    ../../melampig/pluginbase.h \
    reference.h \
    referencestack.h

FORMS += \
    referencestack.ui




