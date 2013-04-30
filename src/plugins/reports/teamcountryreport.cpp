#include "teamcountryreport.h"

#include <QtSql>
#include <QtGui>
#include <orm.h>

namespace Melampig
{

	TeamCountryReport::TeamCountryReport(Object *o, Keeper *k)
		: ReportBase(o,k)
	{
	}

	QString TeamCountryReport::generate(bool ru)
	{
		setReportPath( QString("/../share/reports/%1/team/country").arg(ru ? "ru" : "eng") );
		setReportFile("index.html");

		int cid = comp->get("id").toInt();

		QString sql;
		sql += "SELECT s.id, s.title FROM competition_style cs, style as s WHERE cs.competition = ? AND s.id = cs.style ORDER BY s.title";

		QSqlQuery query(keeper->database());
		query.prepare(sql);
		query.addBindValue(cid);

		if (!query.exec()) {
			QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
		}

		QString page_tmpl   = loadTemplate("page.html");
		QString group_tmpl  = loadTemplate("group.html");
		QString row_tmpl    = loadTemplate("row.html");

		 QMap<QString,QString> pageVars, groupVars, rowVars;
		 QStringList pages, groups, rows;

		while(query.next())
		{
			QString sid    = query.value(0).toString();
			QString stitle = query.value(1).toString();

			sql.clear();
            sql += "select g.id, g.title from competition_team as ct, cgroup as g ";
            sql += "where ct.competition = ? and ct.style = ? and g.id = ct.cgroup group by 1,2 order by g.title";

			QSqlQuery query1(keeper->database());
			query1.prepare(sql);
			query1.addBindValue(cid);
			query1.addBindValue(sid);
			if (!query1.exec()) {
				QMessageBox::critical(0, tr("Error"), query1.executedQuery() + "\n" + query1.lastError().text() );
			}

			while (query1.next()) {
				int grpId = query1.value(0).toInt();
				QString grpTitle = query1.value(1).toString();

				groupVars.insert("{title}", grpTitle);

				sql.clear();
                sql += "select g.alpha3, g.title from competition_team as ct, team as t, geo as g ";
                sql += "where ct.competition = ? and ct.style = ? and ct.cgroup = ? and t.id = ct.team and g.id = t.geo order by ct.sorder";

				QSqlQuery query2(keeper->database());
				query2.prepare(sql);
				query2.addBindValue(cid);
				query2.addBindValue(sid);
				query2.addBindValue(grpId);
				if (!query2.exec()) {
					QMessageBox::critical(0, tr("Error"), query2.executedQuery() + "\n" + query2.lastError().text() );
				}

				while (query2.next()) {
					QString gcode = query2.value(0).toString();
					QString gtitle = query2.value(1).toString();
					rowVars.insert("{code}", gcode.toUpper());
					rowVars.insert("{title}", gtitle);
					rows << applyTemplateVars(row_tmpl, rowVars);
				}

				groupVars.insert("{rows}", rows.join("\n"));
				groups << applyTemplateVars(group_tmpl, groupVars);
				rows.clear();
			}

			pageVars.insert("{groups}", groups.join("\n"));
			pageVars.insert("{style}", stitle);
			pageVars.insert("{competition}", comp->get("title"));
			pages << applyTemplateVars(page_tmpl, pageVars);

			groups.clear();
		}

		vars.insert("{pages}", pages.join("\n"));

		return applyTemplateVars(loadTemplate(reportFile), vars);
	}
}
