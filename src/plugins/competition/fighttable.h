#ifndef FIGHTTABLE_H
#define FIGHTTABLE_H

#include <QMainWindow>
#include <orm.h>

class QLabel;
class QComboBox;
class QTableWidget;
class QToolButton;

namespace Melampig
{
    class FightTable : public QMainWindow
    {
        Q_OBJECT
    public:
        FightTable(Object *o, Keeper *keeper, QWidget *parent = 0);
        ~FightTable();

    signals:

    public slots:
        void styleCombo_changed(int index);
        void searchButton_clicked();

        void updateActions();

    private:
        void populateTable();

        Keeper *keeper;
        QWidget *parentWidget;

        QWidget *centralWidget;

        QLabel    *styleLabel;
        QComboBox *styleCombo;

        QLabel    *weightLabel;
        QComboBox *weightCombo;

        QLabel    *roundLabel;
        QComboBox *roundCombo;

        QTableWidget *tableWidget;

        QToolButton *searchButton;

        Object *comp;

        QHash<int,QVariant> matHash;
    };
}
#endif // FIGHTTABLE_H
