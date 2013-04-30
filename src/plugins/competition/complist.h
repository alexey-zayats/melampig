#ifndef MELAMPIG_CompList_H
#define MELAMPIG_CompList_H


#include "tablewidget.h"

class QWorkspace;
class QAction;

namespace Melampig
{
    class Keeper;
    class MainWindow;

    class CompList : public TableWidget
    {
        Q_OBJECT
        public:
            CompList(TQueryMap &opt, Keeper *keeper, QWidget *parent = 0);
            ~CompList();

        public slots:
            void wrestlerAction_clicked();
            void teamAction_clicked();
            void refereeAction_clicked();
            void memberAction_clicked();

            void tossupAction_clicked();
            void fightAction_clicked();
            void matchAction_clicked();

            void tossupReportAction_clicked();
            void startReportAction_clicked();
            void competitionReportAction_clicked();
            void awardReportAction_clicked();

            void styleAction_clicked();
            void weightAction_clicked();

            void updateActions();

        private:
            QAction *wrestlerAction;
            QAction *teamAction;
            QAction *refereeAction;
            QAction *memberAction;

            QAction *styleAction;
            QAction *weightAction;

            QWidget *parentWidget;
            MainWindow *mainWindow;
            QWorkspace *workspace;

            QAction *tossupAction;
            QAction *fightAction;
            QAction *matchAction;

            QAction *tossupReportAction;
            QAction *startReportAction;
            QAction *fightReportAction;
            QAction *competitionReportAction;
            QAction *competitionReportAction2;
            QAction *awardReportAction;
    };

}
#endif // MELAMPIG_CompList_H
