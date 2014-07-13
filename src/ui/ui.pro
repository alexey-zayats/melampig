TEMPLATE = lib
TARGET = ui

include( ../shared.pri )

TRANSLATIONS =  ui_ru.ts \
		ui_en.ts

DESTDIR = ../../lib
DLLDESTDIR = ../../bin
LIBS +=  -L../../lib
LIBS += -lorm

DEFINES += EXPORT_MELAMPIG_UI
DEFINES += IMPORT_MELAMPIG_ORM

INCLUDEPATH += ../orm
INCLUDEPATH += ../ui
INCLUDEPATH += ../melampig

HEADERS += \
    objectform.h \
    treeview.h \
    tableview.h \
    treemodel.h \
    tablemodel.h \
    ui.h \
    dragdropmodel.h \
    treeitemdelegate.h \
    tableitemdelegate.h \
    svgitemdelegate.h \
    listmodel.h \
    listview.h \
    referencedialog.h \
    ormmodel.h \
    listwidget.h \
    tablewidget.h \
    treewidget.h \
    linkedtablemodel.h \
    linkedtablewidget.h \
    objectselector.h \
    svgeditor.h \
    imageeditor.h \
    datetimeitemdelegate.h \
    dateitemdelegate.h \
    objectrefitemdelegate.h \
    integeritemdelegate.h \
    filterwidget.h \
    logondialog.h \
    projectdialog.h \
    startupdialog.h \
    reportwidget.h \
    intarrayeditor.h \
    objectcompleter.h \
    treemodelcompleter.h \
    referenceslector.h

SOURCES += \
    objectform.cpp \
    treeview.cpp \
    tableview.cpp \
    treemodel.cpp \
    tablemodel.cpp \
    dragdropmodel.cpp \
    treeitemdelegate.cpp \
    tableitemdelegate.cpp \
    svgitemdelegate.cpp \
    listmodel.cpp \
    listview.cpp \
    referencedialog.cpp \
    ormmodel.cpp \
    listwidget.cpp \
    tablewidget.cpp \
    treewidget.cpp \
    linkedtablemodel.cpp \
    linkedtablewidget.cpp \
    objectselector.cpp \
    svgeditor.cpp \
    imageeditor.cpp \
    datetimeitemdelegate.cpp \
    dateitemdelegate.cpp \
    objectrefitemdelegate.cpp \
    integeritemdelegate.cpp \
    filterwidget.cpp \
    logondialog.cpp \
    projectdialog.cpp \
    startupdialog.cpp \
    reportwidget.cpp \
    intarrayeditor.cpp \
    objectcompleter.cpp \
    treemodelcompleter.cpp \
    referenceslector.cpp

FORMS += ../melampig/mainwindow.ui \
    logondialog.ui \
    projectdialog.ui \
    startupdialog.ui
