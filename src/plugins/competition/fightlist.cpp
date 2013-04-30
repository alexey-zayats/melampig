#include "fightlist.h"
#include "tableview.h"
#include "tablemodel.h"

#include <fightsheet.h>

#include <mainwindow.h>
#include <QtGui>

namespace Melampig
{
    FightList::FightList( int com, Keeper *keeper, TQueryMap &opt, QWidget *parent) :
        TableWidget(OFight, keeper, opt, parent),
        parentWidget(parent), competition(com)
    {
        setObjectName( QString("FightList %1").arg(competition) );

        QToolBar *eventBar = addToolBar(tr("Event"));

        scoreSheetAction = eventBar->addAction( QIcon( ":/icons/report.png" ), tr("Scoresheet") );

        connect(scoreSheetAction, SIGNAL(triggered()), this, SLOT(scoreSheetAction_clicked()));

        mainWindow = ((MainWindow*)((QWorkspace*)parentWidget)->parentWidget());
        workspace = (QWorkspace*)parentWidget;

        connect(view->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &,
                                        const QItemSelection &)),
                this, SLOT(updateActions()));

        updateActions();
    }

    FightList::~FightList()
    {}

    void FightList::updateActions()
    {
        bool hasSelection = !view->selectionModel()->selection().isEmpty();
        scoreSheetAction->setEnabled(hasSelection);
        TableWidget::updateActions();
    }

    void FightList::scoreSheetAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title = QString( tr("Scoresheet %1") ).arg(o->get("id"));

		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j )
			if ( windows.at(j)->windowTitle().compare( title ) == 0 )
				return;

		FightSheet *widget = new FightSheet(o, keeper, workspace);
		widget->setWindowTitle(title);
		widget->setAttribute(Qt::WA_DeleteOnClose, true);

		connect( widget, SIGNAL( destroyed() ), mainWindow, SLOT( closeTab() ) );

        workspace->addWindow(widget);
        widget->show();
    }
}
