#ifndef MELAMPIG_WRESTLERTOSSUP_H
#define MELAMPIG_WRESTLERTOSSUP_H

#include <QMainWindow>

#include <orm.h>

class QComboBox;
class QLabel;
class QTableWidget;
class QAction;
class QTableWidgetItem;
class QWorkspace;

namespace Melampig
{
    class MainWindow;
    class WrestlerTossup : public QMainWindow
    {
        Q_OBJECT
    public:
        WrestlerTossup(Object *,Keeper *keeper, QWidget *parent = 0);

    signals:

    public slots:
        void styleCombo_changed(int);
        void weightCombo_changed(int);

        void saveAction_triggerd();
        void autoAction_triggerd();

        void fightsAction_triggerd();

        void tableWidget_itemChanged(QTableWidgetItem*);
        void tableWidget_currentItemChanged();

    private:
        Keeper *keeper;
        Object *object;

        QWidget *parentWidget;

        MainWindow *mainWindow;
        QWorkspace *workspace;

        QWidget *centralWidget;
        QLabel *styleLabel;
        QComboBox *styleCombo;
        QLabel *weightLabel;
        QComboBox *weightCombo;
        QTableWidget *tableWidget;

        QAction *saveAction;
        QAction *autoAction;
        QAction *fightsAction;
        QAction *reportAction;

        QString currentItemText;
    };

}

#endif // MELAMPIG_TOSSUPWIDGET_H
