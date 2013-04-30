#include "teamcompsheet.h"

#include <orm.h>
#include <QtGui>
#include <QtSql>
#include <QtWebKit>

namespace Melampig
{
    TeamCompSheet::TeamCompSheet(Object *o, Keeper *keeper, QWidget *parent) :
        ReportWidget(o, keeper, parent)
    {
        setObjectName(QString("CompSheet:%1:%2").arg(o->objectName()).arg(o->get("id")));

//        pointsBox = new QCheckBox( tr("Show points on classement?"), filterWidget );

//        filerLayout->addWidget(pointsBox, 3, 1, 1, 1);
//        filerLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 3, 2);

//        competitionCombo->setEnabled(false);

        teamLabel->setVisible(false);
        teamCombo->setVisible(false);

        connect(styleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(styleCombo_changed(int)));
    }

    void TeamCompSheet::styleCombo_changed(int /*index*/)
    {
        buildReport->setEnabled(true);
    }

    void TeamCompSheet::genHeader()
    {
        int sid = styleCombo->itemData (styleCombo->currentIndex()).toInt();

        QDate start = QDate::fromString(object->get("start"), QString("yyyy-MM-dd"));
        QDate stop = QDate::fromString(object->get("stop"), QString("yyyy-MM-dd"));

        QString date;
        if ( start.month() == stop.month() ) {
            date = QString("%1-%2/%3/%4").arg(start.day()).arg(stop.day()).arg(start.month()).arg(start.year());
        } else {
            date = start.toString("dd/MM/yyyy") + " - " + stop.toString("dd/MM/yyyy");
        }

        Geo *g = new Geo( object->get("geo").toInt(), keeper);
        Style *s = new Style(sid, keeper);

        vars.insert("{competition}", object->get("title"));

        vars.insert("{date}",   date);
        vars.insert("{geo}",    g->get("title"));
        vars.insert("{style}",  s->get("title"));
    }

    void TeamCompSheet::genCompetition()
    {
        int sid = styleCombo->itemData (styleCombo->currentIndex()).toInt();

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
        opts.insert("group", QStringList() << "cgroup");
        QList<QVariant> cgroupId = keeper->getFieldList(OCompetitionTeam, "cgroup", opts);

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, cgroupId));
        opts.insert("order", QStringList() << "id asc");
        QList<QVariant> cgroupList = keeper->getListOfFields(OCGroup, QStringList() << "title" << "id", opts);

        QString groupTmpl = loadTemplate("team/group.html");
        QString groupCircleTitleTmpl = loadTemplate("team/group-circle-title.html");
        QString groupCircleDataTmpl = loadTemplate("team/group-circle-data.html");
        QString classmentTmpl = loadTemplate("team/classement.html");
        QString classmentRowTmpl = loadTemplate("team/classement-row.html");
        QString matchesTmpl = loadTemplate("team/matches.html");
        QString teamsTmpl = loadTemplate("team/team.html");

        QString matches1Tmpl = loadTemplate("team/matches1.html");
        QString teams1Tmpl = loadTemplate("team/team1.html");

        opts.clear();
        opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
        opts.insert("style", keeper->prepareParam(And|Equal, sid));

        QHash<QString, Object*> compTeamMap = keeper->getObjectMap(OCompetitionTeam, "team", opts);

        QList<QVariant> teamId = keeper->getFieldList(OCompetitionTeam, "team", opts);
        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, teamId));
        QHash<QString, Object*> teamMap = keeper->getObjectMap(OTeam, "id", opts);

        opts.clear();
        QHash<QString, Object*> classifyMap = keeper->getObjectMap(OClassify, "id", opts);

        QMap<QString,QString> tVars;
        QStringList rows;
        for( int i = 0; i < cgroupList.size(); i++ )
        {
            QStringList cgroup = cgroupList.at(i).toStringList();
            int cgrp_id = cgroup.at(1).toInt();
            QString cgrp_title = cgroup.at(0);

            opts.clear();
            opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
            opts.insert("style", keeper->prepareParam(And|Equal, sid));
            opts.insert("cgroup", keeper->prepareParam(And|Equal, cgrp_id));
            QList<QVariant> teamIds = keeper->getFieldList(OCompetitionTeam, "team", opts);

            opts.clear();
            opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
            opts.insert("style", keeper->prepareParam(And|Equal, sid));
            opts.insert("cgroup", keeper->prepareParam(And|Equal, cgrp_id));
            opts.insert("group",QStringList() << "circle");
            opts.insert("order",QStringList() << "circle asc");
            QList<QVariant> circleId = keeper->getFieldList(OMatch, "circle", opts);

            opts.clear();
            opts.insert("id", keeper->prepareParam(InSet, circleId));
            opts.insert("order", QStringList() << "id asc");
            QList<QVariant> circles = keeper->getFieldList(OCircle, "title", opts);

            QStringList groupCirclesTitle;
            QStringList groupCirclesData;

            for ( int j  = 0; j < circles.size(); j++ ) {
                tVars.insert("{circle.title}", circles.at(j).toString());
                groupCirclesTitle << applyTemplateVars(groupCircleTitleTmpl, tVars);
                tVars.clear();
            }

//            int fightNum = Util::factorial(teamIds.size()) / (2 * Util::factorial(teamIds.size() - 2));
//            int circleNum = qFloor(fightNum/2);

            for ( int j  = 0; j < circleId.size(); j++ )
            {
                QStringList matchRow;

                opts.clear();
                opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
                opts.insert("style", keeper->prepareParam(And|Equal, sid));
                opts.insert("cgroup", keeper->prepareParam(And|Equal, cgrp_id));
                opts.insert("circle", keeper->prepareParam(And|Equal, circleId.at(j)));

                QList<Object*> list = keeper->getObjectList(OMatch, opts);

                QList<QVariant> tmp = teamIds;
                for ( int k = 0; k < list.size(); k++ )
                {
                    Object *match = list.at(k);

                    Object *team_a = teamMap[ match->get("team_a") ];
                    Object *team_a_point = compTeamMap[ match->get("team_a") ];

                    Object *team_b = teamMap[ match->get("team_b") ];
                    Object *team_b_point = compTeamMap[ match->get("team_b") ];

                    Object *classify = 0;
                    QString team_a_classify("0");
                    QString team_b_classify("0");
                    if ( classifyMap.contains(match->get("classify")) )
                    {
                        int w = match->get("winner").toInt();
                        int a = match->get("team_a").toInt();
                        int b = match->get("team_b").toInt();
                        classify = classifyMap[ match->get("classify") ];
                        team_a_classify = w == a ? classify->get("win") : classify->get("fail");
                        team_b_classify = w == b ? classify->get("win") : classify->get("fail");
                    }

                    tVars.insert("{team_a.num}",      team_a_point->get("sorder"));
                    tVars.insert("{team_a.title}",    team_a->get("title"));
                    tVars.insert("{team_a.classify}", team_a_classify);
                    tVars.insert("{team_a.win}",      match->get("team_a_win"));

                    tVars.insert("{team_b.num}",      team_b_point->get("sorder"));
                    tVars.insert("{team_b.title}",    team_b->get("title"));
                    tVars.insert("{team_b.classify}", team_b_classify);
                    tVars.insert("{team_b.win}",      match->get("team_b_win"));

                    matchRow << applyTemplateVars(j == 0 ? matchesTmpl : matches1Tmpl, tVars);
                    tVars.clear();

                    tmp.removeAll( team_a->get("id") );
                    tmp.removeAll( team_b->get("id") );
                }

                if ( tmp.size() > 0 ) {
                    Object *team = teamMap[ tmp.at(0).toString() ];
                    Object *team_pnt = compTeamMap[ tmp.at(0).toString() ];
                    tVars.insert("{team.num}",     team_pnt->get("sorder"));
                    tVars.insert("{team.title}",   team->get("title"));
                    tVars.insert("{team.classify}","");
                    tVars.insert("{team.win}",     "");
                    tVars.insert("{team.loose}",   "");
                    matchRow << applyTemplateVars(j == 0 ? teamsTmpl : teams1Tmpl, tVars);
                    tVars.clear();
                }

                tVars.insert("{matches}", matchRow.join("\n"));
                groupCirclesData << applyTemplateVars(groupCircleDataTmpl, tVars);
                tVars.clear();
            }



            tVars.insert("{group.title}", QString("%1").arg(cgrp_title));
            tVars.insert("{group.circles-title}", groupCirclesTitle.join("\n"));
            tVars.insert("{group.circle-data}", groupCirclesData.join("\n"));

            QMap<QString,QString> aVars;
            QStringList aRows;


            QString sql = QString("\
               select \
                  ct.sorder, \
                  t.title, \
                  ct.classify_points, \
                  ct.match_win, \
                  ct.match_loose \
               from \
                  competition_team as ct, \
                  team as t \
               where \
                  ct.competition = ? AND \
                  ct.style = ? AND \
                  ct.cgroup = ? AND \
                  t.id = ct.team \
               order by \
                  ct.classify_points desc, \
                  ct.match_win desc, \
                  ct.match_loose asc");

            QSqlQuery query(keeper->database());
            query.prepare(sql);

            int sid = styleCombo->itemData (styleCombo->currentIndex()).toInt();
            int cid = competitionCombo->itemData(competitionCombo->currentIndex()).toInt();

            query.addBindValue(cid);
            query.addBindValue(sid);
            query.addBindValue(cgrp_id);

            if ( !query.exec() ) {
                qDebug() << sql << cid << sid << cgrp_id;
                QMessageBox::critical(this, tr("Error"), query.lastError().text() );
                return;
            }

            int counter = 1;
            while( query.next() )
            {
                aVars.insert("{counter}", QString::number(counter++));
                aVars.insert("{team.num}", query.value(0).toString());
                aVars.insert("{team.title}", query.value(1).toString());
                aVars.insert("{team.points}", query.value(2).toString());
                aVars.insert("{team.tech}", query.value(3).toString());
                aRows << applyTemplateVars(classmentRowTmpl, aVars);
                aVars.clear();
            }

            aVars.insert("{classement.rows}", aRows.join("\n"));
            aVars.insert("{classement.title}", tr("Classement: %1").arg(cgrp_title));

            tVars.insert("{group.classement}", applyTemplateVars(classmentTmpl, aVars));

            rows << applyTemplateVars(groupTmpl, tVars);
            tVars.clear();
        }

        vars.insert("{groups}", rows.join("\n") );
    }

    void TeamCompSheet::genClassement()
    {
        QStringList rows;
        QMap<QString,QString> tVars;

        QString classmentRowTmpl = loadTemplate("team/classement-row.html");

        QString sql = QString("\
           select \
              ct.sorder, \
              t.title, \
              ct.classify_points, \
              ct.match_win, \
              ct.match_loose \
           from \
              competition_team as ct, \
              team as t \
           where \
              ct.competition = ? AND \
              ct.style = ? AND \
              t.id = ct.team \
           order by \
              ct.classify_points desc, \
              ct.match_win desc, \
              ct.match_loose asc");

        QSqlQuery query(keeper->database());
        query.prepare(sql);

        int sid = styleCombo->itemData (styleCombo->currentIndex()).toInt();
        int cid = competitionCombo->itemData(competitionCombo->currentIndex()).toInt();

        query.addBindValue(cid);
        query.addBindValue(sid);

        if ( !query.exec() ) {
            qDebug() << sql << cid << sid;
            QMessageBox::critical(this, tr("Error"), query.lastError().text() );
            return;
        }

        int counter = 1;
        while( query.next() )
        {
            tVars.insert("{counter}", QString::number(counter++));
            tVars.insert("{team.num}", query.value(0).toString());
            tVars.insert("{team.title}", query.value(1).toString());
            tVars.insert("{team.points}", query.value(2).toString());
            tVars.insert("{team.tech}", query.value(3).toString());
            rows << applyTemplateVars(classmentRowTmpl, tVars);
            tVars.clear();
        }

        QString classmentTmpl = loadTemplate("team/classement.html");
        tVars.insert("{classement.rows}", rows.join("\n"));
        tVars.insert("{classement.title}", tr("Classement"));

        vars.insert("{classement}", applyTemplateVars(classmentTmpl, tVars) );

        QString matchesTmpl = loadTemplate("team/matches.html");

        QList<QVariant> roundName;
        roundName.append("'final'");
        roundName.append("'consolation-1'");

        TQueryMap opts;
        opts.insert("name", keeper->prepareParam(InSet, roundName));
        QList<QVariant> roundIds = keeper->getFieldList(OCRound, "id", opts);

        opts.clear();
        opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
        opts.insert("style", keeper->prepareParam(And|Equal, sid));
        opts.insert("cround", keeper->prepareParam(And|InSet, roundIds));
        opts.insert("order", QStringList() << "cround desc");

        QList<Object*> list = keeper->getObjectList(OMatch, opts);

        opts.clear();
        opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
        opts.insert("style", keeper->prepareParam(And|Equal, sid));

        QHash<QString, Object*> compTeamMap = keeper->getObjectMap(OCompetitionTeam, "team", opts);

        QList<QVariant> teamId = keeper->getFieldList(OCompetitionTeam, "team", opts);
        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, teamId));
        QHash<QString, Object*> teamMap = keeper->getObjectMap(OTeam, "id", opts);

        opts.clear();
        QHash<QString, Object*> classifyMap = keeper->getObjectMap(OClassify, "id", opts);

        for ( int k = 0; k < list.size(); k++ )
        {
            Object *match = list.at(k);

            Object *team_a = teamMap[ match->get("team_a") ];
            Object *team_a_point = compTeamMap[ match->get("team_a") ];

            Object *team_b = teamMap[ match->get("team_b") ];
            Object *team_b_point = compTeamMap[ match->get("team_b") ];

            Object *classify = 0;
            QString team_a_classify("0");
            QString team_b_classify("0");
            if ( classifyMap.contains(match->get("classify")) )
            {
                int w = match->get("winner").toInt();
                int a = match->get("team_a").toInt();
                int b = match->get("team_b").toInt();
                classify = classifyMap[ match->get("classify") ];
                team_a_classify = w == a ? classify->get("win") : classify->get("fail");
                team_b_classify = w == b ? classify->get("win") : classify->get("fail");
            }

            tVars.insert("{team_a.num}",      team_a_point->get("sorder"));
            tVars.insert("{team_a.title}",    team_a->get("title"));
            tVars.insert("{team_a.classify}", team_a_classify);
            tVars.insert("{team_a.win}",      match->get("team_a_win"));

            tVars.insert("{team_b.num}",      team_b_point->get("sorder"));
            tVars.insert("{team_b.title}",    team_b->get("title"));
            tVars.insert("{team_b.classify}", team_b_classify);
            tVars.insert("{team_b.win}",      match->get("team_b_win"));

            vars.insert( k == 0 ? "{final12}" : "{final34}", applyTemplateVars(matchesTmpl, tVars) );
        }

        if ( list.size() == 0 ) {
            tVars.insert("{team_a.num}",      "");
            tVars.insert("{team_a.title}",    "");
            tVars.insert("{team_a.classify}", "");
            tVars.insert("{team_a.win}",      "");

            tVars.insert("{team_b.num}",      "");
            tVars.insert("{team_b.title}",    "");
            tVars.insert("{team_b.classify}", "");
            tVars.insert("{team_b.win}",      "");

            vars.insert( "{final12}", applyTemplateVars(matchesTmpl, tVars) );
            tVars.clear();

            tVars.insert("{team_a.num}",      "");
            tVars.insert("{team_a.title}",    "");
            tVars.insert("{team_a.classify}", "");
            tVars.insert("{team_a.win}",      "");

            tVars.insert("{team_b.num}",      "");
            tVars.insert("{team_b.title}",    "");
            tVars.insert("{team_b.classify}", "");
            tVars.insert("{team_b.win}",      "");

            vars.insert( "{final34}", applyTemplateVars(matchesTmpl, tVars) );
        }
    }

    void TeamCompSheet::genFooter()
    {
        QString sql("\
                 select \
                       r.title, \
                       p.name, \
                       rc.title \
                 from \
                       competition_referee as c, \
                       referee as r, \
                       position as p, \
                       rang as rc \
                 where \
                       c.competition = ? and \
                       r.id = c.referee and \
                       p.id = c.position and \
                       rc.id = r.rang \
                 order by c.id");

        QSqlQuery query(keeper->database());
        query.prepare(sql);
        query.addBindValue(object->get("id"));

        if ( !query.exec() ) {
            QMessageBox::critical(this, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
            return;
        }

        if ( query.size() == 0 ) {
            vars.insert("{referee.chief.category}", "" );
            vars.insert("{referee.chief.title}", "" );
            vars.insert("{secretary.chief.category}", "" );
            vars.insert("{secretary.chief.title}", "" );
        } else {
            while ( query.next() ) {
                QString rtitle = query.value(0).toString();
                QString cname = query.value(2).toString();
                QString pname = query.value(1).toString();
                if ( pname.compare("referee.chief") == 0 ) {
                    vars.insert("{referee.chief.category}", cname );
                    vars.insert("{referee.chief.title}", rtitle );
                } else if ( pname.compare("secretary.chief") == 0 ) {
                    vars.insert("{secretary.chief.category}", cname );
                    vars.insert("{secretary.chief.title}", rtitle );
                }
            }
        }
    }

    QString TeamCompSheet::generateData()
    {
        setReportFile("team.html");
        setReportPath("/../share/reports/competition");

        genHeader();

        genCompetition();
        genClassement();

        genFooter();

        return applyTemplateVars(loadTemplate(reportFile), vars);
    }

}
