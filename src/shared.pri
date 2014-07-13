
QT          += sql svg webkit phonon

DEPENDPATH  += .
INCLUDEPATH += .

win32 {
    PGDIR        = "C:\\Program Files (x86)\\PostgreSQL\\9.3"
    PATH        += "$$PGDIR\\bin"
    INCLUDEPATH += "$$PGDIR\\include"
    LIBS        += "-L$$PGDIR\\lib" -llibpq
}

linux-g++  {
    INCLUDEPATH += /usr/include/postgresql
    LIBS        += -L/usr/lib -lpq
}

unix:QMAKE_RPATHDIR += $$PROJECT_PATH/lib

RESOURCES = $$PROJECT_PATH/melampig.qrc

