#include "personalcountryreport.h"

#include <orm.h>

#include <QtSql>
#include <QtGui>

namespace Melampig
{

	PersonalCountryReport::PersonalCountryReport(Object *o, Keeper *k)
		: ReportBase(o,k)
	{
	}

	QString PersonalCountryReport::generate(bool ru)
	{
		setReportPath( QString("/../share/reports/%1/wrestler/country").arg(ru ? "ru" : "eng") );
		setReportFile("index.html");

		QString page_tmpl = loadTemplate("page.html");
		QString num_tmpl = loadTemplate("num.html");

		QMap<QString,QString> pageVars;
		QStringList pages;

		QString sql;
		sql += "SELECT s.id, s.title FROM competition_style cs, style as s WHERE cs.competition = ? AND s.id = cs.style ORDER BY title";

		QSqlQuery query(keeper->database());
		query.prepare(sql);
		query.addBindValue(comp->get("id"));

		if (!query.exec()) {
			qDebug() << sql << comp->get("id");
			QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
		}

		QHash<QString, QString> style;
		while(query.next()) {
			style.insert(query.value(0).toString(),
						query.value(1).toString());
		}

		pageVars.insert("{competition}", comp->get("title"));
		pageVars.insert("{style}",  QStringList( style.values() ).join("/"));


		sql.clear();
		sql += "SELECT g.id, g.title, alpha3, count(w.id) FROM competition_wrestler as cw, wrestler as w, geo as g ";
		sql += "WHERE cw.competition = ? AND w.id = cw.wrestler AND g.id = w.geo ";
		sql += "GROUP BY 1,2,3 ORDER BY g.title";

		query.prepare(sql);
		query.addBindValue(comp->get("id"));


		if (!query.exec()) {
			qDebug() << sql << comp->get("id");
			QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
		}

		int countryCount = query.size();
		int onPage = 40;

		QStringList rows;
		while ( query.next() )
		{
			QString gtitle = query.value(1).toString();
			QString gcode = query.value(2).toString();

			vars.insert("{gtitle}", gtitle);
			vars.insert("{gcode}", gcode);

			rows << applyTemplateVars( loadTemplate("row.html"), vars );

			if ( rows.size() > onPage ) {
				pageVars.insert("{rows}", rows.join("\n"));
				pageVars.insert("{num}", "");
				pages << applyTemplateVars(page_tmpl, pageVars);
				rows.clear();
			}
		}



		pageVars.insert("{count}", QString::number(countryCount));
		pageVars.insert("{num}", applyTemplateVars(num_tmpl, pageVars));
		pageVars.insert("{rows}", rows.join("\n"));

		pages << applyTemplateVars(page_tmpl, pageVars);

		vars.insert("{pages}", pages.join("\n"));

		return applyTemplateVars(loadTemplate(reportFile), vars);
	}
}
