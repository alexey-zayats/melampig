#include "entrieslistreport.h"

#include <orm.h>

#include <QtSql>
#include <QtGui>

namespace Melampig
{
	EntriesListReport::EntriesListReport(Object *o, Keeper *k)
		: ReportBase(o,k)
	{
	}

	QString EntriesListReport::generate(bool ru)
	{
		setReportPath( QString("/../share/reports/%1/lebc").arg(ru ? "ru" : "eng") );
		setReportFile("index.html");

		int cid = comp->get("id").toInt();

		QString sql;
		sql += "SELECT s.id, s.title FROM competition_style cs, style as s WHERE cs.competition = ? AND s.id = cs.style ORDER BY title";

		QSqlQuery query(keeper->database());
		query.prepare(sql);
		query.addBindValue(cid);

		if (!query.exec()) {
			QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
			return query.executedQuery() + "<br/>\n" + query.lastError().text();
		}

		QMap<QString,QString> wrestlerVars, teamVars, pageVars;
		QStringList pages, teams, rows;

		int countryCount;
		int sid;
		QString stitle;
		TQueryMap opts;

		int wrestlerCount;

		QString team_tmpl = loadTemplate("team.html");
		QString page_tmpl = loadTemplate("page.html");
		QString wrestler_tmpl = loadTemplate("wrestler.html");

		while(query.next())
		{
			sid = query.value(0).toInt();
			stitle = query.value(1).toString();

			qDebug() << stitle;

			pageVars.insert("{competition}", comp->get("title"));
			pageVars.insert("{style}",  stitle);

			sql.clear();
			sql += "SELECT g.id, g.title, alpha3, count(w.id) ";
			sql += "FROM competition_wrestler as cw, ";
			sql += "wrestler as w, ";
			sql += "geo as g ";
			sql += "WHERE cw.competition = ? AND cw.style = ? AND w.id = cw.wrestler AND g.id = w.geo ";
			sql += "GROUP BY 1,2,3 ORDER BY g.title";

			QSqlQuery query1(keeper->database());
			query1.prepare(sql);
			query1.addBindValue(cid);
			query1.addBindValue(sid);

			if (!query1.exec()) {
				QMessageBox::critical(0, tr("Error"), query1.executedQuery() + "\n" + query1.lastError().text() );
				continue;
			}

			countryCount = query1.size();

			opts.clear();
			opts.insert("competition", keeper->prepareParam(Equal, QString::number(cid)));
			opts.insert("style", keeper->prepareParam(And|Equal, QString::number(sid)));

			wrestlerCount = keeper->countObjects(OCompetitionWrestler, opts);

			sql.clear();
			sql += "SELECT wh.title, wr.lastname, wr.firstname FROM competition_wrestler as cw, wrestler as wr, weight as wh ";
			sql += "WHERE cw.competition = :competition AND cw.style = :style AND wr.id = cw.wrestler AND wh.id = cw.weight AND wr.geo = :geo ";
			sql += "ORDER BY 1,2,3";

			QSqlQuery nQuery(keeper->database());
			nQuery.prepare(sql);
			nQuery.bindValue("competition", cid);
			nQuery.bindValue("style", sid);

			wrestlerCount += countryCount*3;
			int itemPerPage = 45;

			int k = 0;
			while ( query1.next() )
			{
				int gid = query1.value(0).toInt();
				QString gtitle = query1.value(1).toString();
				QString gcode = query1.value(2).toString();

				k += 3;

				qDebug() << stitle << gtitle << k;

				teamVars.insert("{team.title}", gtitle);
				teamVars.insert("{team.geo}", gcode);

				teamVars.insert("{team.leader}", "");
				teamVars.insert("{team.coach}", "");
				teamVars.insert("{team.doctor}", "");
				teamVars.insert("{team.physio}", "");

				nQuery.bindValue("geo", gid);

				if (!nQuery.exec()) {
					QMessageBox::critical(0, tr("Error"), nQuery.executedQuery() + "\n" + nQuery.lastError().text() );
				}

				while ( nQuery.next() )
				{
					k++;

					qDebug() << nQuery.value(0).toString() << nQuery.value(1).toString().toUpper() << k;

					wrestlerVars.insert("{weight}", nQuery.value(0).toString());
					wrestlerVars.insert("{fio}", QString("%1, %2").
														arg(nQuery.value(1).toString().toUpper()).
														arg(nQuery.value(2).toString()) );
					rows << applyTemplateVars(wrestler_tmpl, wrestlerVars);

					if ( k > itemPerPage )
					{
						k = 3;
						teamVars.insert("{wrestlers}", rows.join("\n"));
						rows.clear();

						teams << applyTemplateVars(team_tmpl, teamVars);
						pageVars.insert("{teams}", teams.join("\n"));
						teams.clear();

						pages << applyTemplateVars(page_tmpl, pageVars);
					}
				};

				teamVars.insert("{wrestlers}", rows.join("\n"));
				rows.clear();

				teams << applyTemplateVars(team_tmpl, teamVars);
				teamVars.clear();
			}

			pageVars.insert("{teams}", teams.join("\n"));
			pages << applyTemplateVars(page_tmpl, pageVars);
			teams.clear();
		}

		vars.insert("{pages}", pages.join("\n"));

		return applyTemplateVars(loadTemplate(reportFile), vars);
	}
}

