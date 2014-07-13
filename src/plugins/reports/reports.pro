
TEMPLATE = lib

include ( ../../shared.pri )

TRANSLATIONS =  reports_ru.ts \
                reports_en.ts
CONFIG += dll
QT +=xml

DEFINES += IMPORT_MELAMPIG_ORM
DEFINES += IMPORT_MELAMPIG_UI

INCLUDEPATH += ../../melampig
INCLUDEPATH += ../../orm
INCLUDEPATH += ../../ui

DESTDIR = ../../../lib
LIBS +=  -L../../../lib

LIBS += -lorm
LIBS += -lui

TARGET = reports

SOURCES += \
    reports.cpp \
    listreports.cpp \
    frontpagereport.cpp \
    entrieslistreport.cpp \
    entriesnumreport.cpp \
    personalcountryreport.cpp \
    personalscorereport.cpp \
    refereelistreport.cpp \
    teamcountryreport.cpp \
    teamscorereport.cpp \
    reportbase.cpp \
    xmlreport.cpp \
    settingsform.cpp

HEADERS += \
    ../../melampig/pluginbase.h \
    reports.h \
    listreports.h \
    frontpagereport.h \
    entrieslistreport.h \
    entriesnumreport.h \
    personalcountryreport.h \
    personalscorereport.h \
    refereelistreport.h \
    teamcountryreport.h \
    teamscorereport.h \
    reportbase.h \
    xmlreport.h \
    settingsform.h

FORMS += \
    listreports.ui \
    xmlreport.ui \
    settingsform.ui











