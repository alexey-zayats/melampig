#include "teamweighing.h"

#include <orm.h>
#include <QtGui>
#include <QtWebKit>
#include <QtSql>

namespace Melampig
{
    TeamWeighing::TeamWeighing(Object *o, Keeper *keeper, QWidget *parent) :
        ReportWidget(o, keeper, parent)
    {
        setObjectName(QString("TeamWeighing:%1:%2").arg(o->objectName()).arg(o->get("id")));
//        competitionCombo->setEnabled(false);
    }

    QString TeamWeighing::generateData()
    {
        QString path = QString("/../share/reports/%1/team/weighing").arg(reportLang);
        qDebug() << path;

        setReportPath(path);
        setReportFile("index.html");

        int sid = styleCombo->itemData(styleCombo->currentIndex()).toInt();
        int cid = competitionCombo->itemData(competitionCombo->currentIndex()).toInt();
        int tid = teamCombo->itemData( teamCombo->currentIndex() ).toInt();

        vars.insert("{competition}", object->get("title"));

        QDate start = QDate::fromString(object->get("start"), QString("yyyy-MM-dd"));
        QDate stop = QDate::fromString(object->get("stop"), QString("yyyy-MM-dd"));

        QString date;
        if ( start.month() == stop.month() ) {
            date = QString("%1-%2/%3/%4").arg(start.day()).arg(stop.day()).arg(start.month()).arg(start.year());
        } else {
            date = start.toString("dd/MM/yyyy") + " - " + stop.toString("dd/MM/yyyy");
        }

        vars.insert("{date}", date);

        Geo *g = new Geo( object->get("geo").toInt(), keeper);
        vars.insert("{geo}", g->get("title"));

        Team *t = new Team( tid, keeper);
        vars.insert("{team}", t->get("title"));

        Style *s = new Style( sid, keeper);
        vars.insert("{style}", s->get("title"));

        QString sql;
        sql.append("\
               select \
                   wh.title, \
                   w.title, \
                   gw.title, \
                   extract( year from w.birthdate) as birthdate, \
                   rw.acr, \
                   ca.title, \
                   c.heft \
               from \
                   competition_wrestler as c \
                   left join wrestler as w on w.id = c.wrestler \
                   left join weight as wh on wh.id = c.weight \
                   left join geo as gw on gw.id = w.geo \
                   left join rang as rw on rw.id = w.rang \
                   left join coach as ca on ca.id = w.coach \
              where \
                   c.competition = ? AND \
                   c.team = ? AND \
                   c.style = ? \
              order by \
                   wh.title");

        QSqlQuery query(keeper->database());

        query.prepare(sql);

        query.addBindValue(cid);
        query.addBindValue(tid);
        query.addBindValue(sid);

        if ( !query.exec() ) {
            QMessageBox::critical(this, tr("Error"), query.lastQuery() + "\n" + query.lastError().text() );
            return QString();
        }


        int size = query.size();

        QMap<QString,QString> tVars;
        QStringList pages;

        int itemPerPage = 45;
        int numPages = ( size % itemPerPage ) == 0 ? (size/itemPerPage) : qCeil(size/itemPerPage)+1;

        QString page = loadTemplate("page.html");

        for( int i = 0; i < numPages; i++)
        {
            QStringList rows;
            for( int k = 0; k < itemPerPage; k++)
            {
                if ( !query.next() ) break;

                rows << "<tr>";
                rows << QString("<td>%1</td>").arg( ++i );
                rows << QString("<td>%1</td>").arg( query.value(0).toString() );
                rows << QString("<td>%1</td>").arg( query.value(1).toString() );
                rows << QString("<td>%1</td>").arg( query.value(2).toString() );
                rows << QString("<td>%1</td>").arg( query.value(3).toString() );
                rows << QString("<td>%1</td>").arg( query.value(4).toString() );
                rows << QString("<td>%1</td>").arg( query.value(5).toString() );
                rows << QString("<td>%1</td>").arg( query.value(6).toString() );
                rows << "</tr>";

            }
            tVars.insert("{rows}", rows.join("\n"));
            tVars.insert("{competition}", object->get("title"));
            pages << applyTemplateVars(page, tVars);
        }

        vars.insert("{pages}", pages.join("\n") );

        sql = QString ("\
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
                order \
                       by c.id");

        query.prepare(sql);
        query.addBindValue(cid);

        if ( !query.exec() ) {
            QMessageBox::critical(this, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
            return QString();
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

        delete g;
        delete t;
        delete s;

        return applyTemplateVars(loadTemplate(reportFile), vars);
    }

}
