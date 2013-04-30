#include "wrestlerweighing.h"

#include <orm.h>
#include <QtGui>
#include <QtWebKit>
#include <QtSql>

namespace Melampig
{
    WrestlerWeighing::WrestlerWeighing(Object *o, Keeper *keeper, QWidget *parent) :
        ReportWidget(o, keeper, parent)
    {
        setObjectName(QString("WrestlerWeighing:%1:%2").arg(o->objectName()).arg(o->get("id")));
    }

    QString WrestlerWeighing::generateData()
    {
        setReportPath( QString("/../share/reports/%1/wrestler/weighing").arg(reportLang) );
        setReportFile("index.html");

        int wid = weightCombo->itemData(weightCombo->currentIndex()).toInt();
        int sid = styleCombo->itemData(styleCombo->currentIndex()).toInt();
        int cid = competitionCombo->itemData(competitionCombo->currentIndex()).toInt();

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

        Weight *w = new Weight(wid, keeper);
        vars.insert("{weight}", w->get("title"));

        Geo *g = new Geo( object->get("geo").toInt(), keeper);
        vars.insert("{geo}", g->get("title"));

        QString page = loadTemplate("page.html");

        QString sql;
        sql.append("\
               select \
                   c.sorder, \
                   w.title, \
                   gw.title, \
                   rw.acr, \
                   c.heft, \
                   extract( year from w.birthdate) as birthdate, \
                   ca.title as coach \
               from \
                   competition_wrestler as c \
                   left join wrestler as w on w.id = c.wrestler \
                   left join coach as ca on ca.id = w.coach \
                   left join team as t on t.id = c.team \
                   left join geo as gw on gw.id = w.geo \
                   left join geo as gt on gt.id = t.geo \
                   left join rang as rw on rw.id = w.rang \
              where \
                   c.competition = ? AND \
                   c.weight = ? AND \
                   c.style = ? \
              order by \
                   c.sorder");

        QSqlQuery query(keeper->database());

        query.prepare(sql);

        query.addBindValue(cid);
        query.addBindValue(wid);
        query.addBindValue(sid);

        if ( !query.exec() ) {
            QMessageBox::critical(this, tr("Error"), query.lastQuery() + "\n" + query.lastError().text() );
            return QString();
        }

        int size = query.size();

        QMap<QString,QString> tVars;
        QStringList pages;

        int itemPerPage = 40;
        int numPages = ( size % itemPerPage ) == 0 ? (size/itemPerPage) : qCeil(size/itemPerPage)+1;

        for( int i = 0; i < numPages; i++)
        {
            QStringList rows;
            for( int k = 0; k < itemPerPage; k++)
            {
                if ( !query.next() ) break;

                rows << "<tr>";
                rows << QString("<td>%1</td>").arg( query.value(0).toString() );
                rows << QString("<td>%1</td>").arg( query.value(1).toString() );
                rows << QString("<td>%1</td>").arg( query.value(2).toString() );
                if ( reportLang.compare( tr("ru") ) == 0 )
                    rows << QString("<td>%1</td>").arg( query.value(3).toString() );
                rows << QString("<td>%1</td>").arg( query.value(5).toString() );
                rows << QString("<td>%1</td>").arg( query.value(6).toString() );
                rows << QString("<td>%1</td>").arg( query.value(4).toString() );
                rows << "</tr>";

            }
            tVars.insert("{rows}", rows.join("\n"));
            tVars.insert("{competition}", object->get("title"));
            pages << applyTemplateVars(page, tVars);
        }

        vars.insert("{pages}", pages.join("\n") );

        if ( reportLang.compare( "ru" ) == 0 )
            refereeList();

        delete w;
        delete g;

        return applyTemplateVars(loadTemplate(reportFile), vars);
    }

}
