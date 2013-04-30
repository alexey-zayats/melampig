#ifndef MELAMPIG_FIGHTLIST_H
#define MELAMPIG_FIGHTLIST_H

#include "tablewidget.h"
#include <orm.h>

class QComboBox;
class QLabel;
class QAction;
class QWorkspace;

namespace Melampig
{
   class TableWidget;
   class MainWindow;

   class FightList : public TableWidget
   {
       Q_OBJECT
   public:
       FightList(int com, Keeper *keeper, TQueryMap &opt, QWidget *parent = 0);
       ~FightList();

   public slots:
       void scoreSheetAction_clicked();
       void updateActions();

   private:
       QWidget *parentWidget;
       QWorkspace *workspace;

       int competition;

       QAction *scoreSheetAction;
       QAction *competitionSheetAction;

       MainWindow *mainWindow;
   };

}

#endif // FIGHTLIST_H
