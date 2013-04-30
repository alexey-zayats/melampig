#include "frontpagereport.h"

#include <orm.h>

#include <QDate>

namespace Melampig
{
	FrontPageReport::FrontPageReport(Object *o, Keeper *k)
		: ReportBase(o,k)
	{
	}

	QString FrontPageReport::generate(bool ru)
	{
		QDate start = QDate::fromString(comp->get("start"), QString("yyyy-MM-dd"));
		QDate stop = QDate::fromString(comp->get("stop"), QString("yyyy-MM-dd"));

		QString date;
		if ( start.month() == stop.month() ) {
			date = QString("%1-%2/%3/%4").arg(start.day()).arg(stop.day()).arg(start.month()).arg(start.year());
		} else {
			date = start.toString("dd/MM/yyyy") + " - " + stop.toString("dd/MM/yyyy");
		}

		Geo *g = new Geo( comp->get("geo").toInt(), keeper);

		TQueryMap opt;
		opt.insert("competition", keeper->prepareParam(Equal, comp->get("id")));
		QList<QVariant> idList = keeper->getFieldList(OCompetitionStyle, "style", opt);

		opt.clear();
		opt.insert("id", keeper->prepareParam(InSet, idList));
		QList<Object*> objList = keeper->getObjectList(OStyle, opt);

		QStringList style;
		for( int i = 0; i < objList.size(); i++ ) {
			style << objList.at(i)->get("title");
		}

		vars.insert("{competition}", comp->get("title"));
		vars.insert("{date}",   date);
		vars.insert("{geo}",    g->get("title"));
		vars.insert("{style}",  style.join("<br/>"));

		setReportPath( QString("/../share/reports/%1/front").arg(ru ? "ru" : "eng") );
		setReportFile("index.html");

		delete g;

		return applyTemplateVars(loadTemplate(reportFile), vars);
	}
}
