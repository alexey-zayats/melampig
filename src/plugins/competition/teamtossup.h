#ifndef TEAMTOSSUPWIDGET_H
#define TEAMTOSSUPWIDGET_H

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
    class TeamTossup : public QMainWindow
    {
        Q_OBJECT
        public:
            TeamTossup(Object *,Keeper *keeper, QWidget *parent = 0);

        public slots:
            void styleCombo_changed(int);
//            void groupCombo_changed(int);

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

//            QLabel *groupLabel;
//            QComboBox *groupCombo;

            QTableWidget *tableWidget;

            QAction *saveAction;
            QAction *autoAction;
            QAction *fightsAction;
            QAction *reportAction;

            QString currentItemText;
    };
}

#endif // TEAMTOSSUPWIDGET_H
