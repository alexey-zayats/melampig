
TEMPLATE = lib

include( ../shared.pri )

TRANSLATIONS =  orm_ru.ts \
		orm_en.ts 

DESTDIR = ../../lib
DLLDESTDIR = ../../bin

RESOURCES = ../../melampig.qrc
LIBS +=  -L../../lib

TARGET = orm

DEFINES += EXPORT_MELAMPIG_ORM

HEADERS += \
    object.h \
    exports.h \
    errors.h \
    orm.h \
    util.h \
    keeper.h \
    attr.h \
    tree.h \
    defines.h \
    wrestler.h \
    geo.h \
    link.h \
    age.h \
    weight.h \
    style.h \
    rang.h \
    position.h \
    action.h \
    team.h \
    referee.h \
    classify.h \
    competition.h \
    member.h \
    role.h \
    mat.h \
    fight.h \
    fight_log.h \
    competitionreferee.h \
    competitionmember.h \
    project.h \
    coach.h \
    addresstype.h \
    circle.h \
    competitionstyle.h \
    cround.h \
    ctour.h \
    competitionteam.h \
    competitionweight.h \
    fround.h \
    gender.h \
    match.h \
    comptype.h \
    memberaddress.h \
    coachaddress.h \
    wrestleraddress.h \
    refereeaddress.h \
    competitionwrestler.h \
    cgroup.h

SOURCES += \
    object.cpp \
    util.cpp \
    keeper.cpp \
    attr.cpp \
    tree.cpp \
    wrestler.cpp \
    geo.cpp \
    link.cpp \
    age.cpp \
    weight.cpp \
    style.cpp \
    rang.cpp \
    position.cpp \
    action.cpp \
    team.cpp \
    referee.cpp \
    classify.cpp \
    competition.cpp \
    member.cpp \
    role.cpp \
    mat.cpp \
    fight.cpp \
    fight_log.cpp \
    competitionreferee.cpp \
    competitionmember.cpp \
    project.cpp \
    coach.cpp \
    addresstype.cpp \
    circle.cpp \
    competitionstyle.cpp \
    cround.cpp \
    ctour.cpp \
    competitionteam.cpp \
    competitionweight.cpp \
    fround.cpp \
    gender.cpp \
    match.cpp \
    comptype.cpp \
    memberaddress.cpp \
    coachaddress.cpp \
    wrestleraddress.cpp \
    refereeaddress.cpp \
    competitionwrestler.cpp \
    cgroup.cpp
