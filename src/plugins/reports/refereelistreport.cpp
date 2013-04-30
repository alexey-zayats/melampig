#include "refereelistreport.h"

#include <orm.h>
#include <QtSql>
#include <QtGui>

namespace Melampig
{

	RefereeListReport::RefereeListReport(Object *o, Keeper *k)
		: ReportBase(o,k)
	{
	}

	QString RefereeListReport::generate(bool ru)
	{
		QString sql;
		sql += "SELECT s.title FROM competition_style cs, style as s WHERE cs.competition = ? AND s.id = cs.style ORDER BY title";

		QSqlQuery query(keeper->database());
		query.prepare(sql);
		query.addBindValue(comp->get("id"));

		if (!query.exec()) {
			qDebug() << sql << comp->get("id");
			QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
		}

		QStringList style;
		while(query.next()) {
			style << query.value(0).toString();
		}

		vars.insert("{competition}", comp->get("title"));
		vars.insert("{style}",  style.join("/"));

		setReportPath( QString("/../share/reports/%1/referee").arg(ru ? "ru" : "eng") );
		setReportFile("index.html");

		sql.clear();
		sql += "SELECT r.license, g.title, r.lastname, r.firstname, p.title ";
		sql += "FROM competition_referee as c ";
		sql += "LEFT JOIN referee as r ON r.id = c.referee ";
		sql += "LEFT JOIN geo as g ON g.id = r.geo ";
		sql += "LEFT JOIN position as p ON p.id = c.position ";
		sql += "WHERE c.competition = ?";
		sql += "ORDER BY r.title";

		query.clear();
		query.prepare(sql);
		query.addBindValue(comp->get("id"));

		if (!query.exec()) {
			qDebug() << sql << comp->get("id");
			QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
		}

		int size = query.size();
		int itemPerPage = 40;
		int numPages = ( size % itemPerPage ) == 0 ? (size/itemPerPage) : qCeil(size/itemPerPage)+1;

		int j = 0;
		QStringList pages;
		for( int i = 0; i < numPages; i++)
		{
			QStringList rows;
			for( int k = 0; k < itemPerPage; k++)
			{
				if ( !query.next() ) break;

				QString row;
				row += "<tr>";
				row += QString("<td>%1</td>").arg(++j);
				row += QString("<td>%1</td>").arg(query.value(0).toString());
				row += QString("<td>%1</td>").arg(query.value(1).toString());
				row += QString("<td>%1</td>").arg(query.value(2).toString());
				row += QString("<td>%1</td>").arg(query.value(3).toString());
				if ( ru )
					row += QString("<td>%1</td>").arg(query.value(4).toString());
				row += "</tr>";
				rows << row;
			}
			vars.insert("{rows}", rows.join("\n"));
			if ( i == (numPages-1) ) {
				vars.insert("{count}", QString::number(size) );
				vars.insert("{refnum}", applyTemplateVars(loadTemplate("refnum.html"), vars) );
			} else {
				vars.insert("{refnum}", "" );
			}
			pages << applyTemplateVars(loadTemplate("page.html"), vars);
		}

		vars.insert("{pages}", pages.join("\n"));

		return applyTemplateVars(loadTemplate(reportFile), vars);
	}
}
