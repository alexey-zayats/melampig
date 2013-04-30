#ifndef MELAMPIG_MATCHLIST_H
#define MELAMPIG_MATCHLIST_H

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

   class MatchList : public TableWidget
   {
       Q_OBJECT
   public:
       MatchList(int com, Keeper *keeper, TQueryMap &opt, QWidget *parent = 0);
       ~MatchList();

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

#endif // MELAMPIG_MATCHLIST_H
