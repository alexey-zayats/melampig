#include "entriesnumreport.h"

#include <orm.h>
#include <QtSql>
#include <QtGui>

namespace Melampig
{

	EntriesNumReport::EntriesNumReport(Object *o, Keeper *k)
		: ReportBase(o,k)
	{
	}


	QString EntriesNumReport::generate(bool ru)
	{
		setReportPath( QString("/../share/reports/%1/nebc").arg(ru ? "ru" : "eng") );
		setReportFile("index.html");

		QMap<QString,QString> pageVars, headVars, rowVars, totalVars, weightVars, itemVars;
		QStringList rows, pageRows, headRows, weightRows, itemRows;

		QString page_tmpl    = loadTemplate("page.html");
		QString head_tmpl    = loadTemplate("head.html");
		QString row_tmpl     = loadTemplate("row.html");
		QString total_tmpl   = loadTemplate("total.html");
		QString weight_tmpl  = loadTemplate("weight.html");
		QString num_tmpl  = loadTemplate("num.html");

		QString sql;
		sql += "SELECT w.id, w.title FROM competition_weight cw, weight as w ";
		sql += "WHERE cw.competition = ? AND w.id = cw.weight ";
		sql += "ORDER BY w.title";

		QSqlQuery query(keeper->database());
		query.prepare(sql);
		query.addBindValue(comp->get("id"));

		if (!query.exec()) {
			QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
		}

		QMap<int, QString> weight;
		while(query.next()) {
			weight.insert(query.value(0).toInt(), query.value(1).toString());
		}

		sql.clear();
		sql += "SELECT s.id, s.title ";
		sql += "FROM competition_style cs, style as s ";
		sql += "WHERE cs.competition = ? AND s.id = cs.style ORDER BY title";

		query.prepare(sql);
		query.addBindValue(comp->get("id"));

		if (!query.exec()) {
			QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
			return query.executedQuery() + "<br/>\n" +
					query.lastError().text();
		}

		while(query.next())
		{
			int sid = query.value(0).toInt();
			QString stitle = query.value(1).toString();

			QMap<int,QString>::iterator it = weight.begin();
			for(; it != weight.end(); it++) {
				headVars.insert("{weight}", it.value());
				headRows << applyTemplateVars(head_tmpl, headVars);
			}

			pageVars.insert("{competition}", comp->get("title"));
			pageVars.insert("{style}", stitle);
			pageVars.insert("{head}", headRows.join("\n"));

			headRows.clear();

			sql.clear();
			sql += "SELECT g.id, g.title, alpha3, count(w.id) ";
			sql += "FROM competition_wrestler as cw, wrestler as w, geo as g ";
			sql += "WHERE cw.competition = ? AND cw.style = ? AND w.id = cw.wrestler AND g.id = w.geo ";
			sql += "GROUP BY 1,2,3 ORDER BY g.title";

			QSqlQuery query1(keeper->database());
			query1.prepare(sql);
			query1.addBindValue(comp->get("id"));
			query1.addBindValue(sid);

			if (!query1.exec()) {
				QMessageBox::critical(0, tr("Error"), query1.executedQuery() + "\n" + query1.lastError().text() );
				continue;
			}

			int countryCount = query1.size();
			int itemPerPage = 30;

			sql.clear();
			sql += "select c.weight, count(w.id) ";
			sql += "from ";
			sql += "(select id, wrestler, weight from competition_wrestler where competition = :competition and style = :style) as c, ";
			sql += "(select id, title from wrestler where geo = :geo) as w ";
			sql += "WHERE c.wrestler = w.id GROUP BY c.weight";

			QSqlQuery query2(keeper->database());
			query2.prepare(sql);
			query2.bindValue("competition", comp->get("id"));
			query2.bindValue("style", sid);

			QMap<QString,int> total;

			int pos = 0;
			while ( query1.next() )
			{
				int gid = query1.value(0).toInt();
				QString gtitle = query1.value(1).toString();
				QString gcode = query1.value(2).toString();
				QString gcount = query1.value(3).toString();

				query2.bindValue("geo", gid);

				if (!query2.exec()) {
					QMessageBox::critical(0, tr("Error"), query2.executedQuery() + "\n" + query2.lastError().text() );
					continue;
				}

				QHash<QString, QString> gw;
				while ( query2.next() ) {
					gw.insert(query2.value(0).toString(), query2.value(1).toString());
				}

				for( int i = 0; i < weight.keys().size(); ++i )
				{
					QString key = QString::number( weight.keys().at(i) );

					if ( gw.contains(key) ) {
						if ( total.contains(key) )
							total[key] += gw[key].toInt();
						else
							total.insert(key, gw[key].toInt());
					}

					weightVars.insert("{count}", gw.contains(key) ? gw[key] : "" );
					weightRows << applyTemplateVars(weight_tmpl, weightVars);
					weightVars.clear();
				}

				rowVars.insert("{gcode}", gcode);
				rowVars.insert("{gtitle}", gtitle);
				rowVars.insert("{gcount}", gcount);
				rowVars.insert("{wcount}", weightRows.join(""));
				weightRows.clear();

				rows << applyTemplateVars(row_tmpl, rowVars);
				rowVars.clear();

				pos++;

				if ( pos > itemPerPage ) {
					pos = 0;

					pageVars.insert("{cnum}", "");
					pageVars.insert("{total}", "");
					pageVars.insert("{rows}", rows.join("\n"));

					pageRows << applyTemplateVars(page_tmpl, pageVars);
					rows.clear();
				}
			}

			pageVars.insert("{rows}", rows.join("\n"));
			rows.clear();

			int summ = 0;
			for( int i = 0; i < weight.keys().size(); ++i )
			{
				QString key = QString::number( weight.keys().at(i) );

				if ( total.contains(key) ) {
					summ += total[key];
					itemVars.insert("{count}", QString::number(total[key]));
				} else {
					itemVars.insert("{count}", "" );
				}

				itemRows << applyTemplateVars(weight_tmpl, itemVars);
				itemVars.clear();
			}
			totalVars.insert("{items}", itemRows.join("\n"));
			totalVars.insert("{summ}", QString::number(summ));

			itemRows.clear();

			itemVars.insert("{count}", QString::number(countryCount) );
			pageVars.insert("{cnum}", applyTemplateVars(num_tmpl, itemVars) );
			itemRows.clear();

			pageVars.insert("{total}", applyTemplateVars(total_tmpl, totalVars));
			pageRows << applyTemplateVars(page_tmpl, pageVars);
		}

		vars.insert("{pages}", pageRows.join("\n"));
		return applyTemplateVars(loadTemplate(reportFile), vars);
	}
}
