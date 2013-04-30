#ifndef MELAMPIG_XMLREPORT_H
#define MELAMPIG_XMLREPORT_H

#include "ui_xmlreport.h"

#include <QMainWindow>
#include <orm.h>

namespace Ui {
    class XmlReport;
}

class QDomDocument;

namespace Melampig
{
    class Object;
    class Keeper;
    class Attr;
    class ObjectSelector;

    class XmlReport : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit XmlReport(Keeper *keeper, QWidget *parent = 0);
        ~XmlReport();

    public slots:
        void competitionSelected(Object*);
        void fileChoose_clicked();
        void generateButton_clicked();
        void saveButton_clicked();

    private:
        Ui::XmlReport ui;

        Keeper *keeper;
        ObjectSelector *os;
        Object *comp;

        Attr *attr;

        QDomDocument *getReport();
    };
}

#endif // MELAMPIG_XMLREPORT_H
