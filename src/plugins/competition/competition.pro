TEMPLATE = lib

include ( $$PROJECT_PATH/src/shared.pri )

TRANSLATIONS =  competition_ru.ts \
		competition_en.ts
CONFIG += dll

DESTDIR = $$PROJECT_PATH/lib
LIBS +=  -L$$PROJECT_PATH/lib

LIBS += -lorm
LIBS += -lui

TARGET = competition

DEFINES += IMPORT_MELAMPIG_ORM
DEFINES += IMPORT_MELAMPIG_UI

INCLUDEPATH += $$PROJECT_PATH/src/melampig
INCLUDEPATH += $$PROJECT_PATH/src/orm
INCLUDEPATH += $$PROJECT_PATH/src/ui

SOURCES += \
    competition.cpp \
    complist.cpp \
    fightlist.cpp \
    awardreport.cpp \
    teamtossup.cpp \
    wrestlertossup.cpp \
    wrestlerweighing.cpp \
    teamweighing.cpp \
    teammatchprotocol.cpp \
    wrestlerstartprotocol.cpp \
    teamcompsheet.cpp \
    matchlist.cpp \
    matchsheet.cpp \
    fightsheet.cpp \
    wrestlercompsheet.cpp

HEADERS += \
    ../../melampig/pluginbase.h \
    competition.h \
    complist.h \
    fightlist.h \
    awardreport.h \
    wrestlertossup.h \
    teamtossup.h \
    wrestlerweighing.h \
    teamweighing.h \
    teammatchprotocol.h \
    wrestlerstartprotocol.h \
    teamcompsheet.h \
    matchlist.h \
    matchsheet.h \
    fightsheet.h \
    wrestlercompsheet.h

FORMS +=





























































