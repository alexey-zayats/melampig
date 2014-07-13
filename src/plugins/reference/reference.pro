
TEMPLATE = lib

include ( ../../shared.pri )

TRANSLATIONS =  reference_ru.ts \
		reference_en.ts
CONFIG += dll

DEFINES += IMPORT_MELAMPIG_ORM
DEFINES += IMPORT_MELAMPIG_UI

INCLUDEPATH += ../../melampig
INCLUDEPATH += ../../orm
INCLUDEPATH += ../../ui

DESTDIR = ../../../lib
LIBS +=  -L../../../lib

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




