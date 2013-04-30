#include "fightsheet.h"

#include <orm.h>
#include <QtGui>
#include <QtWebKit>

namespace Melampig
{
    FightSheet::FightSheet(Object *o, Keeper *keeper, QWidget *parent) :
        ReportWidget(o, keeper, parent)
    {
        setObjectName(QString("FightSheet:%1:%2").arg(o->objectName()).arg(o->get("id")));
        dockWidget->close();

        generateReport( generateData() );
    }

    QString FightSheet::generateData()
    {
        Style *s = new Style( object->get("style").toInt(), keeper );

        setReportFile(s->get("name").toLower() + ".html");
        setReportPath("/../share/reports/fightsheet");

        Wrestler *red = new Wrestler(object->get("red").toInt(), keeper);
        Geo *geo_r = new Geo(red->get("geo").toInt(), keeper);
        Wrestler *blue = new Wrestler(object->get("blue").toInt(), keeper);
        Geo *geo_b = new Geo(blue->get("geo").toInt(), keeper);
        Weight *wh = new Weight( object->get("weight").toInt(), keeper );

        Competition *c = new Competition(object->get("competition").toInt(), keeper);

        Mat *mat = new Mat(object->get("mat").toInt(), keeper);
        CRound *cround = new CRound(object->get("cround").toInt(), keeper);
        CTour *ctour = new CTour(object->get("ctour").toInt(), keeper);

        QDate start = QDate::fromString(c->get("start"), QString("yyyy-MM-dd"));
        QDate stop = QDate::fromString(c->get("stop"), QString("yyyy-MM-dd"));

        QString date;
        if ( start.month() == stop.month() ) {
            date = QString("%1-%2/%3/%4").arg(start.day()).arg(stop.day()).arg(start.month()).arg(start.year());
        } else {
            date = start.toString("dd/MM/yyyy") + " - " + stop.toString("dd/MM/yyyy");
        }

        vars.insert("{red.title}", red->get("title"));
        vars.insert("{red.geo}", geo_r->get("title"));
        vars.insert("{blue.title}", blue->get("title"));
        vars.insert("{blue.geo}", geo_b->get("title"));
        vars.insert("{style}", s->get("name"));
        vars.insert("{weight}", wh->get("title"));
        vars.insert("{competition}", c->get("title"));
        vars.insert("{mat}", mat->get("title"));
        vars.insert("{round}", cround->get("title"));
        vars.insert("{place}", ctour->get("title"));
        vars.insert("{num}", object->get("num"));
        vars.insert("{date}", date);

        TQueryMap opt;
        opt.insert("competition", keeper->prepareParam(Equal, c->get("id")));

        QList<QVariant> id_list;
        if ( red->get("id").toInt() > 0 )
            id_list << red->get("id");

        if ( blue->get("id").toInt() > 0 )
            id_list << blue->get("id");

        opt.insert("wrestler", keeper->prepareParam(And | InSet, id_list ));
        opt.insert("style", keeper->prepareParam(And | Equal, s->get("id")));
        opt.insert("weight", keeper->prepareParam(And | Equal, wh->get("id")));


        QList<QVariant> list = keeper->getListOfFields(OCompetitionWrestler,
                                                       QStringList() << "wrestler" << "sorder", opt);
        for( int i = 0; i < list.size(); i++)
        {
            QStringList row = list.at(i).toStringList();
            if ( red->get("id").toInt() == row.at(0).toInt() )
                vars.insert("{red.num}", row.at(1));
            if ( blue->get("id").toInt() == row.at(0).toInt() )
                vars.insert("{blue.num}", row.at(1));
        }

        delete red;
        delete geo_r;
        delete geo_b;
        delete blue;
        delete s;
        delete wh;
        delete mat;
        delete cround;
        delete ctour;
        delete c;

        return applyTemplateVars(loadTemplate(reportFile), vars);
    }

}
