#include "complist.h"

#include <QtGui>
#include <ui.h>
#include <orm.h>

#include "fightlist.h"
#include "matchlist.h"

#include "wrestlertossup.h"
#include "teamtossup.h"

#include "mainwindow.h"

#include "wrestlerweighing.h"
#include "wrestlerstartprotocol.h"
#include "wrestlercompsheet.h"

#include "teamweighing.h"
#include "teammatchprotocol.h"
#include "teamcompsheet.h"

#include "awardreport.h"

namespace Melampig
{
    CompList::CompList(TQueryMap &opt, Keeper *keeper, QWidget *parent) :
        TableWidget(OCompetition, keeper, opt, parent), parentWidget(parent)
    {
        QToolBar *toolBar = addToolBar(tr("tools"));

        wrestlerAction = toolBar->addAction( QIcon( ":/icons/user_red.png" ), tr("Wrestlers") );
        teamAction = toolBar->addAction( QIcon( ":/icons/group.png" ), tr("Teams") );

        refereeAction = toolBar->addAction( QIcon( ":/icons/user_suit.png" ), tr("Referee") );
        memberAction = toolBar->addAction( QIcon( ":/icons/user_comment.png" ), tr("Members") );

        toolBar->addSeparator();

        styleAction = toolBar->addAction( QIcon( ":/icons/css.png" ), tr("Style") );
        weightAction = toolBar->addAction( QIcon( ":/icons/drive_web.png" ), tr("Weight") );

        connect(styleAction, SIGNAL(triggered()), this, SLOT(styleAction_clicked()));
        connect(weightAction, SIGNAL(triggered()), this, SLOT(weightAction_clicked()));

        toolBar->addSeparator();

        tossupAction = toolBar->addAction( QIcon( ":/icons/asterisk_orange.png" ), tr("Toss-up") );
        fightAction = toolBar->addAction( QIcon( ":/icons/shield.png" ), tr("Fights") );
        matchAction = toolBar->addAction( QIcon( ":/icons/shape_group.png" ), tr("Matches") );

        connect(wrestlerAction, SIGNAL(triggered()), this, SLOT(wrestlerAction_clicked()));
        connect(teamAction, SIGNAL(triggered()), this, SLOT(teamAction_clicked()));
        connect(refereeAction, SIGNAL(triggered()), this, SLOT(refereeAction_clicked()));
        connect(memberAction, SIGNAL(triggered()), this, SLOT(memberAction_clicked()));

        connect(tossupAction, SIGNAL(triggered()), this, SLOT(tossupAction_clicked()));
        connect(fightAction, SIGNAL(triggered()), this, SLOT(fightAction_clicked()));
        connect(matchAction, SIGNAL(triggered()), this, SLOT(matchAction_clicked()));

        toolBar->addSeparator();

        tossupReportAction = toolBar->addAction(QIcon( ":/icons/report_link.png" ), tr("Toss-up report"));
        startReportAction = toolBar->addAction(QIcon( ":/icons/report_go.png" ), tr("Start protocol report"));
        competitionReportAction = toolBar->addAction(QIcon( ":/icons/report_key.png" ), tr("Competition report"));

        awardReportAction = toolBar->addAction(QIcon( ":/icons/award_star_gold_1.png" ), tr("Award card report"));

        connect(tossupReportAction, SIGNAL(triggered()), this, SLOT(tossupReportAction_clicked()));
        connect(startReportAction, SIGNAL(triggered()), this, SLOT(startReportAction_clicked()));
        connect(competitionReportAction, SIGNAL(triggered()), this, SLOT(competitionReportAction_clicked()));
        connect(awardReportAction, SIGNAL(triggered()), this, SLOT(awardReportAction_clicked()));

        mainWindow = ((MainWindow*)((QWorkspace*)parentWidget)->parentWidget());
        workspace = (QWorkspace*)parentWidget;

        connect(view->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &,
                                        const QItemSelection &)),
                this, SLOT(updateActions()));

        updateActions();
    }

    CompList::~CompList()
    {
    }

    void CompList::updateActions()
    {
        bool hasSelection = !view->selectionModel()->selection().isEmpty();
        bool byteam = false;

        if ( hasSelection ) {
            Object *o = model->getObject(view->selectionModel()->currentIndex());
            if ( o )
                byteam = o->get("byteam").compare("true") == 0;
        }

        wrestlerAction->setEnabled(hasSelection);
        teamAction->setEnabled(hasSelection);
        refereeAction->setEnabled(hasSelection);
        memberAction->setEnabled(hasSelection);

        tossupAction->setEnabled(hasSelection);
        fightAction->setEnabled(hasSelection);
        matchAction->setEnabled(byteam && hasSelection);

        tossupReportAction->setEnabled(hasSelection);
        startReportAction->setEnabled(!byteam && hasSelection);
        competitionReportAction->setEnabled(!byteam && hasSelection);
        awardReportAction->setEnabled(!byteam && hasSelection);

        styleAction->setEnabled(hasSelection);
        weightAction->setEnabled(hasSelection);

        TableWidget::updateActions();
    }

    void CompList::styleAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Style / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        TQueryMap opt;
        opt.insert("competition", keeper->prepareParam(Equal, o->get("id")));
        TableWidget *lt = new TableWidget(OCompetitionStyle, keeper, opt, workspace);

        lt->setAttribute( Qt::WA_DeleteOnClose, true );
        lt->setWindowTitle( title );

        workspace->addWindow(lt);

        lt->show();
    }

    void CompList::weightAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Weight / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        TQueryMap opt;
        opt.insert("competition", keeper->prepareParam(Equal, o->get("id")));
        TableWidget *lt = new TableWidget(OCompetitionWeight, keeper, opt, workspace);

        lt->setAttribute( Qt::WA_DeleteOnClose, true );
        lt->setWindowTitle( title );

        workspace->addWindow(lt);

        lt->show();
    }

    void CompList::wrestlerAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Wrestler / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        TQueryMap opt;
        opt.insert("competition", keeper->prepareParam(Equal, o->get("id")));
        TableWidget *lt = new TableWidget(OCompetitionWrestler, keeper, opt, workspace);

        lt->setAttribute( Qt::WA_DeleteOnClose, true );
        lt->setWindowTitle( title );

        workspace->addWindow(lt);

        lt->show();
    }

    void CompList::teamAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Team / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        TQueryMap opt;
        opt.insert("competition", keeper->prepareParam(Equal, o->get("id")));
        TableWidget *lt = new TableWidget(OCompetitionTeam, keeper, opt, workspace);

        lt->setAttribute( Qt::WA_DeleteOnClose, true );
        lt->setWindowTitle( title );

        workspace->addWindow(lt);

        lt->show();
    }

    void CompList::refereeAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Referee / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        TQueryMap opt;
        opt.insert("competition", keeper->prepareParam(Equal, o->get("id")));
        TableWidget *lt = new TableWidget(OCompetitionReferee, keeper, opt, workspace);
        lt->setAttribute( Qt::WA_DeleteOnClose, true );
        lt->setWindowTitle( title );


        workspace->addWindow(lt);
        lt->show();
    }

    void CompList::memberAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Members / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        TQueryMap opt;
        opt.insert("competition", keeper->prepareParam(Equal, o->get("id")));
        TableWidget *lt = new TableWidget(OCompetitionMember, keeper, opt, workspace);
        lt->setAttribute( Qt::WA_DeleteOnClose, true );
        lt->setWindowTitle( title );


        workspace->addWindow(lt);
        lt->show();
    }

    void CompList::tossupAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Toss-up / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        QWidget *w = 0;
        if ( o->get("byteam").compare("true") == 0 ) {
           w = new TeamTossup(o, keeper, workspace);
        } else {
           w = new WrestlerTossup(o, keeper, workspace);
        }

        w->setWindowTitle(title);
        w->setAttribute(Qt::WA_DeleteOnClose, true);

        workspace->addWindow(w);
        w->show();
    }

    void CompList::fightAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Fights / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, o->get("id")));
        opts.insert("order", QStringList() << "num asc");

        FightList *fl = new FightList(o->get("id").toInt(), keeper, opts, workspace);
        fl->setWindowTitle(title);
        fl->setAttribute(Qt::WA_DeleteOnClose, true);

        workspace->addWindow(fl);
        fl->show();
    }

    void CompList::matchAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title( tr("Matches / %1").arg(o->get("title")) );

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, o->get("id")));
        opts.insert("order", QStringList() << "num asc");

        MatchList *w = new MatchList(o->get("id").toInt(), keeper, opts, workspace);
        w->setWindowTitle(title);
        w->setAttribute(Qt::WA_DeleteOnClose, true);

        workspace->addWindow(w);
        w->show();
    }

    void CompList::tossupReportAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());

        QString title = QString( tr("Weighing Protocol / %1") )
                .arg(o->get("title"));

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        QWidget *w = 0;
        if ( o->get("byteam").compare("true") == 0 ) {
           w = new TeamWeighing(o, keeper, workspace);
        } else {
           w = new WrestlerWeighing(o, keeper, workspace);
        }
        w->setWindowTitle(title);
        w->setAttribute(Qt::WA_DeleteOnClose, true);

        workspace->addWindow(w);
        w->show();
    }

    void CompList::startReportAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());
        QString title = QString( tr("Start Protocol / %1") ).arg(o->get("title"));

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        QWidget *w = 0;
        if ( o->get("byteam").compare("true") == 0 ) {
//           w = new TeamMatchProtocol(o, keeper, workspace);
            return;
        } else {
           w = new WrestlerStartProtocol(o, keeper, workspace);
        }

        w->setWindowTitle(title);
        w->setAttribute(Qt::WA_DeleteOnClose, true);

        workspace->addWindow(w);
        w->show();
    }

    void CompList::competitionReportAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());

        QString title = QString( tr("Competition Sheet / %1") ).arg(o->get("title"));

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        QWidget *w = 0;
        if ( o->get("byteam").compare("true") == 0 ) {
           w = new TeamCompSheet(o, keeper, workspace);
        } else {
           w = new WrestlerCompSheet(o, keeper, workspace);
        }
        w->setWindowTitle(title);
        w->setAttribute(Qt::WA_DeleteOnClose, true);

        workspace->addWindow(w);
        w->show();
    }

    void CompList::awardReportAction_clicked()
    {
        Object *o = model->getObject(view->selectionModel()->currentIndex());

        QString title = QString( tr("Award Card / %1") ).arg(o->get("title"));

        QWidgetList windows = workspace->windowList();
        for ( int j = 0; j < windows.size(); ++j ) {
            if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
                workspace->setActiveWindow(windows.at(j));
                return;
            }
        }

        AwardReport *ar = new AwardReport(o, keeper, workspace);
        ar->setWindowTitle(title);
        ar->setAttribute(Qt::WA_DeleteOnClose, true);

        workspace->addWindow(ar);
        ar->show();
    }

}
