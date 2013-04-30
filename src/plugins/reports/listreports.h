#ifndef MELAMPIG_LISTREPORTS_H
#define MELAMPIG_LISTREPORTS_H

#include <QUrl>
#include <QMainWindow>
#include <QTextDocument>
#include <QModelIndex>

#include "ui_listreports.h"

class QWebView;
class QLineEdit;
class QToolButton;
class QToolBar;
class QAction;
class QKeyEvent;
class QListWidget;

namespace Melampig
{
    class Object;
    class Keeper;
    class Attr;
    class ObjectSelector;

    class ListReports : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit ListReports(Keeper *keeper, QWidget *parent = 0);
        ~ListReports();

    signals:

    public slots:
            void competitionSelected(Object*);

            void generateReport();
            void printPdf();
            void printPreview();

            void ruRadio_clicked();
            void engRadio_clicked();

    protected:
            void keyPressEvent ( QKeyEvent *);

    private:
            Ui::ListReports ui;
            ObjectSelector *os;
            Keeper *keeper;
            Object *comp;

            bool isRuLang;
            bool isByTeam;

            Attr *attr;
    };

}

#endif // MELAMPIG_LISTREPORTS_H

