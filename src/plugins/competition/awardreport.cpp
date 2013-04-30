#include "awardreport.h"

#include <orm.h>
#include <QtGui>
#include <QtWebKit>
#include <QtSql>

namespace Melampig
{
    AwardReport::AwardReport(Object *o, Keeper *keeper, QWidget *parent) :
        ReportWidget(o, keeper, parent)
    {
        setObjectName(QString("AwardReport:%1:%2").arg(o->objectName()).arg(o->get("id")));

        weightLabel->hide();
        weightCombo->hide();

        teamLabel->hide();
        teamCombo->hide();
    }

    QString AwardReport::generateData()
    {
        setReportFile("index.html");
        setReportPath( QString("/../share/reports/%1/wrestler/award").arg(reportLang) );

        int sid = styleCombo->itemData (styleCombo->currentIndex()).toInt();
//        int wid = weightCombo->itemData(weightCombo->currentIndex()).toInt();
        int cid = competitionCombo->itemData(competitionCombo->currentIndex()).toInt();

        QString page_t = loadTemplate("page.html");
        QString award_t = loadTemplate("award.html");
        QString row_t = loadTemplate("row.html");

        object->restore(cid);

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


        vars.insert("{date}",   date);
        vars.insert("{geo}",    g->get("title"));
        vars.insert("{style}",  s->get("name"));

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, object->get("id")));
        opts.insert("order", QStringList() << "weight asc");
        QList<QVariant> wids = keeper->getFieldList(OCompetitionWeight, "weight", opts );

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, wids));
        opts.insert("order", QStringList() << "title asc");
        QList<Object *> wlist = keeper->getObjectList(OWeight, opts );

        QMap<QString,QString> tVars;
        QStringList pages;

        int size = wlist.size();

        int itemPerPage = 3;
        int numPages = ( size % itemPerPage ) == 0 ? (size/itemPerPage) : qCeil(size/itemPerPage)+1;

        int wid = 0;
        Weight *w = 0;

        for( int i = 0; i < numPages; i++)
        {
            QStringList awards;
            for( int k = 0; k < itemPerPage; k++)
            {
                if ( wlist.size() == 0 ) break;

                w = (Weight*)(wlist.takeAt(0));
                wid = w->get("id").toInt();

                QString sql = QString("\
                   select \
                      c.sorder, \
                      w.title, \
                      extract( year from w.birthdate), \
                      r.name, \
                      g.title, \
                      ca.title \
                   from \
                      competition_wrestler as c, \
                      wrestler as w \
                        left join coach as ca on ca.id = w.coach, \
                      geo as g, \
                      rang as r, \
                      wrestler_classement(?, ?, ?) as cc \
                   where \
                      c.competition = ? AND \
                      c.weight = ? AND \
                      c.style = ? AND \
                      w.id = c.wrestler AND \
                      g.id = w.geo AND \
                      r.id = w.rang AND \
                      intset(c.wrestler) && cc \
                   order by \
                      idx(cc,w.id) \
                   LIMIT 4");

                QSqlQuery query(keeper->database());
                query.prepare(sql);

                query.addBindValue(cid);
                query.addBindValue(wid);
                query.addBindValue(sid);

                query.addBindValue(cid);
                query.addBindValue(wid);
                query.addBindValue(sid);

                if ( !query.exec() ) {
                    qDebug() << sql << object->get("id") << wid << sid;
                    QMessageBox::critical(this, tr("Error"), query.lastError().text() + "\n" + query.executedQuery() );
                    continue;
                }

                QMap<QString,QString> tVars;

                QStringList rows;
                int counter = 1;
                while( query.next() )
                {
                    tVars.insert("{counter}", QString::number((counter == 4 ? counter - 1 : counter)));
                    tVars.insert("{wrestler.num}", query.value(0).toString());
                    tVars.insert("{wrestler.fio}", query.value(1).toString());
                    tVars.insert("{wrestler.year}", query.value(2).toString());
                    if ( reportLang.compare("ru") == 0 )
                        tVars.insert("{wrestler.rang}", query.value(3).toString());
                    tVars.insert("{wrestler.geo}", query.value(4).toString());
                    tVars.insert("{wrestler.coach}", query.value(5).toString());
                    rows << applyTemplateVars(row_t, tVars);
                    tVars.clear();
                    counter++;
                }
                tVars.insert("{weight}", w->get("title"));
                tVars.insert("{rows}", rows.join("\n"));
                awards << applyTemplateVars(award_t, tVars);
                tVars.clear();
                delete w;
            }
            tVars.insert("{awards}", awards.join("\n"));
            tVars.insert("{competition}", object->get("title"));
            pages << applyTemplateVars(page_t, tVars);
            tVars.clear();
        }
        vars.insert("{pages}", pages.join("\n") );

        delete g;
        delete s;

        return applyTemplateVars(loadTemplate(reportFile), vars);
    }

}
