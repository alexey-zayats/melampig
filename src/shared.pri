
QT          += sql svg webkit phonon

DEPENDPATH  += .
INCLUDEPATH += .

win32 {
    LIBS        += -llibpq
}

linux-g++  {
    LIBS        += -lpq
}

unix:QMAKE_RPATHDIR += $$PROJECT_PATH/lib

RESOURCES = $$PROJECT_PATH/melampig.qrc

