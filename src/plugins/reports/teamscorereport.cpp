#include "teamscorereport.h"

#include <QtSql>
#include <QtGui>

#include <orm.h>

namespace Melampig
{

    TeamScoreReport::TeamScoreReport(Object *o, Keeper *k)
        : ReportBase(o,k)
    {

    }

    QString TeamScoreReport::generate(bool ru)
    {
        setReportPath( QString("/../share/reports/%1/team/competition").arg(ru ? "ru" : "eng") );
        setReportFile("index.html");

        competition_id = comp->get("id").toInt();
        competition_title = comp->get("title");

        tmpls.insert("tour",   loadTemplate("tour.html"));
        tmpls.insert("style",  loadTemplate("style.html"));
        tmpls.insert("rating", loadTemplate("rating.html"));
        tmpls.insert("weight", loadTemplate("weight.html"));

        // стили соревнования
        QString styleSql = "SELECT s.id, s.title FROM competition_style cs, style as s WHERE cs.competition = ? AND s.id = cs.style ORDER BY s.title";
        QSqlQuery style(keeper->database());
        style.prepare(styleSql);

        // получаем информацию о стилях в соревновании
        style.bindValue(0, competition_id);

        if (!style.exec()) {
            QMessageBox::critical(0, tr("Error"), style.executedQuery() + "\n" + style.lastError().text() );
            return style.executedQuery() + "<br/>\n" + style.lastError().text();
        }

        QStringList styleRows;

        while(style.next())
        {
            style_id    = style.value(0).toInt();
            style_title = style.value(1).toString();

            styleVars.insert("{competition}", competition_title);
            styleVars.insert("{style}", style_title);

            reportByRound(false);
            groupRanking();
            reportByRound(true);
            finalRanking();

            styleRows << applyTemplateVars(tmpls["style"], styleVars);
        }

        vars.insert("{styles}", styleRows.join("\n"));
        styleRows.clear();

        return applyTemplateVars(loadTemplate(reportFile), vars);
    }

    void TeamScoreReport::reportByRound(bool is_final)
    {

        QMap<QString,QString> tourVars, weightVars;
        QStringList tourRows, weightRows;

        // круги соревнования до финалов по стилю
        QString circleRSql = "select c.id, c.title as circle from match as m left join circle as c on c.id = m.circle where m.competition = ? and m.style = ? and c.name in ('circle-1', 'circle-2', 'circle-3', 'circle-4', 'circle-5') group by 1,2 order by c.title";
        QSqlQuery circleRound(keeper->database());
        circleRound.prepare(circleRSql);

        // круги соревнования финалов по стилю
        QString circleFSql = "select c.id, c.title as circle from match as m left join circle as c on c.id = m.circle where m.competition = ? and m.style = ? and c.name in ('final-1-2', 'final-3-4', 'final-5-6', 'final-7-8', 'final-9-10') group by 1,2 order by c.id desc";
        QSqlQuery circleFinal(keeper->database());
        circleFinal.prepare(circleFSql);

        // матчи соревнования по кругам и стилю
        QString matchSql = "select m.id as match, g.title as group, c.title as circle, t.title as winner, c.name, m.team_a_win, m.team_b_win, case m.winner when m.team_a then true else false end as a_win from match as m left join cgroup as g on g.id = m.cgroup left join circle as c on c.id = m.circle left join team as t on t.id = m.winner where m.competition = ? and m.style = ? and c.id = ? order by g.title, c.title";
        QSqlQuery match(keeper->database());
        match.prepare(matchSql);

        // схватки соревнования по стилю и матчу
        QString fightSql = "SELECT w.title AS weight, ct1.sorder AS red_num, CASE WHEN f.red = f.winner THEN cf.win ELSE cf.fail END AS red_classify, w1.title AS red_fio, g1.title AS red_geo, f.red_score AS red_score, ct2.sorder blue_num, CASE WHEN f.blue = f.winner THEN cf.win ELSE cf.fail END AS blue_classify, w2.title AS blue_fio, g2.title AS blue_geo, f.blue_score AS blue_score, g3.title AS winner FROM fight AS f LEFT JOIN weight AS w ON w.id = f.weight LEFT JOIN wrestler AS w1 ON w1.id = f.red LEFT JOIN geo AS g1 ON g1.id = w1.geo LEFT JOIN competition_wrestler AS cw1 ON cw1.competition = f.competition and cw1.style = f.style and cw1.wrestler = f.red LEFT JOIN team AS t1 ON t1.id = cw1.team LEFT JOIN competition_team AS ct1 ON ct1.team = t1.id and ct1.competition = f.competition and ct1.style = f.style LEFT JOIN wrestler AS w2 ON w2.id = f.blue LEFT JOIN geo AS g2 ON g2.id = w2.geo LEFT JOIN competition_wrestler AS cw2 ON cw2.competition = f.competition and cw2.style = f.style and cw2.wrestler = f.blue LEFT JOIN team AS t2 ON t2.id = cw2.team LEFT JOIN competition_team AS ct2 ON ct2.team = t2.id and ct2.competition = f.competition and ct2.style = f.style LEFT JOIN wrestler AS w3 ON w3.id = f.winner LEFT JOIN geo AS g3 ON g3.id = w3.geo LEFT JOIN classify AS cf ON cf.id = f.classify WHERE f.competition = ? AND f.style = ? AND f.match = ? ORDER BY f.num";
        QSqlQuery fights(keeper->database());
        fights.prepare(fightSql);

        // круги соревнования до финалов

        QSqlQuery query = is_final ? circleFinal : circleRound;

        query.bindValue(0, competition_id);
        query.bindValue(1, style_id);

        qDebug() << (is_final ? circleRSql : circleFSql) << competition_id << style_id;

        if (!query.exec()) {
            QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
        }

        int circle_id;
        QString circle_title;

        while( query.next() )
        {
            circle_id = query.value(0).toInt();
            circle_title = query.value(1).toInt();

            // матчи по кругам до финалов
            match.bindValue(0, competition_id);
            match.bindValue(1, style_id);
            match.bindValue(2, circle_id);

            qDebug() << matchSql << competition_id << style_id << circle_id;

            int match_id;
            QString tour_title, group_title, match_winner;
            // m.id as match, g.title as group, c.title as circle, t.title as winner, c.name
            if (!match.exec()) {
                QMessageBox::critical(0, tr("Error"), match.executedQuery() + "\n" + match.lastError().text() );
            }

            QString a,b;
            bool a_win;
            while( match.next() )
            {
                match_id     = match.value(0).toInt();
                group_title  = match.value(1).toString();
                tour_title   = match.value(2).toString();
                match_winner = match.value(3).toString();
                a            = match.value(5).toString();
                b            = match.value(6).toString();
                a_win        = match.value(7).toBool();

                tourVars.insert("{tour}", tour_title);
                tourVars.insert("{group}", group_title);
                tourVars.insert("{winner}", match_winner);
                tourVars.insert("{score}", a_win ? QString("%1:%2").arg(a).arg(b) : QString("%1:%2").arg(b).arg(a) );

                tourVars.insert("{competition}", competition_title);
                tourVars.insert("{style}", style_title);

                // схватки по матчам до финалов
                fights.bindValue(0, competition_id);
                fights.bindValue(1, style_id);
                fights.bindValue(2, match_id);

                qDebug() << fightSql << competition_id << style_id << match_id;

                if (!fights.exec()) {
                    QMessageBox::critical(0, tr("Error"), fights.executedQuery() + "\n" + fights.lastError().text() );
                }

                // fights
                QString weight_title,
                        red_num, red_classify, red_fio, red_geo,
                        blue_num, blue_classify, blue_fio, blue_geo,
                        winner_title;

                QStringList red_score, blue_score;
                QString red_score_sum, blue_score_sum;

                while (fights.next())
                {
                    weight_title = fights.value(0).toString();

                    red_num      = fights.value(1).toString();
                    red_classify = fights.value(2).toString();
                    red_fio      = fights.value(3).toString();
                    red_geo      = fights.value(4).toString();

                    red_score = Object::toList(fights.value(5).toString());
                    red_score_sum = QString::number(red_score.at(0).toInt() +
                                                  red_score.at(1).toInt() +
                                                  red_score.at(2).toInt());

                    blue_num      = fights.value(6).toString();
                    blue_classify = fights.value(7).toString();
                    blue_fio      = fights.value(8).toString();
                    blue_geo      = fights.value(9).toString();

                    blue_score = Object::toList(fights.value(10).toString());
                    blue_score_sum = QString::number(blue_score.at(0).toInt() +
                                                   blue_score.at(1).toInt() +
                                                   blue_score.at(2).toInt());

                    winner_title = fights.value(11).toString();

                    weightVars.insert("{weight}", weight_title);

                    weightVars.insert("{red.num}", red_num);
                    weightVars.insert("{red.classify}", red_classify);
                    weightVars.insert("{red.fio}", red_fio);
                    weightVars.insert("{red.geo}", red_geo);
                    weightVars.insert("{red.sum}", red_score_sum);
                    weightVars.insert("{red.p1}", red_score.at(0));
                    weightVars.insert("{red.p2}", red_score.at(1));
                    weightVars.insert("{red.p3}", red_score.at(2));

                    weightVars.insert("{blue.num}", blue_num);
                    weightVars.insert("{blue.classify}", blue_classify);
                    weightVars.insert("{blue.fio}", blue_fio);
                    weightVars.insert("{blue.geo}", blue_geo);
                    weightVars.insert("{blue.sum}", blue_score_sum);
                    weightVars.insert("{blue.p1}", blue_score.at(0));
                    weightVars.insert("{blue.p2}", blue_score.at(1));
                    weightVars.insert("{blue.p3}", blue_score.at(2));

                    weightVars.insert("{winner}", winner_title);
                    weightRows << applyTemplateVars(tmpls["weight"], weightVars);

                    weightVars.clear();
                }

                tourVars.insert("{weights}", weightRows.join("\n"));
                tourRows << applyTemplateVars(tmpls["tour"], tourVars);
                weightRows.clear();
            }
        }


        styleVars.insert( (is_final ? "{finals}" : "{groups}"), tourRows.join("\n"));
        tourRows.clear();
    }

    void TeamScoreReport::groupRanking() {}

    void TeamScoreReport::finalRanking()
    {
        QStringList ratingRows;
        QMap<QString,QString> ratingVars;

        QString sql = "SELECT g1.alpha3, g1.title, g2.alpha3, g2.title, c.name FROM circle as c, match as m LEFT JOIN team as t1 on t1.id = m.winner left join geo as g1 on g1.id = t1.geo left join team as t2 on t2.id = (case when m.winner = m.team_a then m.team_b else m.team_a end)  left join geo as g2 on g2.id = t2.geo where m.competition = ? and m.style = ? and c.name in ('final-1-2', 'final-3-4', 'final-5-6', 'final-7-8', 'final-9-10') and m.circle = c.id ORDER BY c.name";

        QSqlQuery query(keeper->database());
        query.prepare(sql);

        query.bindValue(0, competition_id);
        query.bindValue(1, style_id);

        qDebug() << sql << competition_id, style_id;

        if (!query.exec()) {
            QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
        }

        int p = 1;
        while ( query.next() ) {

            ratingVars.insert("{place}", place(p++));
            ratingVars.insert("{geo.code}", query.value(0).toString());
            ratingVars.insert("{geo.title}", query.value(1).toString());
            ratingRows << applyTemplateVars(tmpls["rating"], ratingVars);

            ratingVars.insert("{place}", place(p++));
            ratingVars.insert("{geo.code}", query.value(2).toString());
            ratingVars.insert("{geo.title}", query.value(3).toString());
            ratingRows << applyTemplateVars(tmpls["rating"], ratingVars);
        }

        styleVars.insert("{rating}", ratingRows.join("\n"));
    }

    QString TeamScoreReport::place(int num)
    {
        QString place;
        switch (num) {
            case 1: place = tr("Gold"); break;
            case 2: place = tr("Silver"); break;
            case 3: place = tr("Bronze"); break;
            default: place = QString::number(num); break;
        }
        return place;
    }
}
