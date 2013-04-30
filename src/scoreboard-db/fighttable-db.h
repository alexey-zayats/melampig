#ifndef FIGHTTABLE_H
#define FIGHTTABLE_H

#include <QDialog>
#include <orm.h>

class QLabel;
class QComboBox;
class QTableWidget;
class QToolButton;
class QPushButton;

namespace Melampig
{
    class FightTable : public QDialog
    {
        Q_OBJECT
    public:
        FightTable(Keeper *keeper, QWidget *parent = 0);
        ~FightTable();
        int getFightId();

    signals:

    public slots:
        void compCombo_changed(int);
        void styleCombo_changed(int index);
        void searchButton_clicked();
        void lockFight();
        void tableWidget_itemSelectionChanged ();

        void updateActions();

    private:
        void populateTable();

        Keeper *keeper;
        QWidget *parentWidget;

        QLabel    *styleLabel;
        QComboBox *styleCombo;

        QLabel    *weightLabel;
        QComboBox *weightCombo;

        QLabel    *matchLabel;
        QComboBox *matchCombo;

        QLabel    *matLabel;
        QComboBox *matCombo;

        QLabel *compLabel;
        QComboBox *compCombo;

        QTableWidget *tableWidget;

        QToolButton *searchButton;

        Object *comp;

        QHash<int,QVariant> matHash;
        QHash<int,QVariant> roundHash;
        QHash<int,QVariant> tourHash;

        QPushButton *okButton;
        QPushButton *cancelButton;
    };
}
#endif // FIGHTTABLE_H
