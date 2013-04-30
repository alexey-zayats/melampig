#include "matchsheet.h"

#include <orm.h>
#include <QtGui>
#include <QtWebKit>
#include <QtSql>

namespace Melampig
{
    MatchSheet::MatchSheet(Object *o, Keeper *keeper, QWidget *parent) :
        ReportWidget(o, keeper, parent)
    {
        setObjectName(QString("MatchSheet:%1:%2").arg(o->objectName()).arg(o->get("id")));

        competitionLabel->hide();
        competitionCombo->hide();

        styleLabel->hide();
        styleCombo->hide();

        weightLabel->hide();
        weightCombo->hide();

        teamLabel->hide();
        teamCombo->hide();
//        dockWidget->close();

        generateReport( generateData() );
    }

    QString MatchSheet::generateData()
    {

        QString path = QString("/../share/reports/%1/team/matchsheet").arg(reportLang);
        setReportPath( path );
        setReportFile("index.html");

        // object -> match

        Competition *c = new Competition(object->get("competition").toInt(), keeper);
        vars.insert("{competition}", c->get("title"));

        QDate start = QDate::fromString(c->get("start"), QString("yyyy-MM-dd"));
        QDate stop = QDate::fromString(c->get("stop"), QString("yyyy-MM-dd"));

        QString date;
        if ( start.month() == stop.month() ) {
            date = QString("%1-%2/%3/%4").arg(start.day()).arg(stop.day()).arg(start.month()).arg(start.year());
        } else {
            date = start.toString("dd/MM/yyyy") + " - " + stop.toString("dd/MM/yyyy");
        }
        vars.insert("{date}", date);

        Style *s = new Style(object->get("style").toInt(), keeper);
        vars.insert("{style}", s->get("title"));

        Geo *g = new Geo(c->get("geo").toInt(), keeper);

        vars.insert("{geo}", g->get("title"));

        vars.insert("{match.num}", object->get("num"));

        Circle *cl = new Circle(object->get("circle").toInt(), keeper);
        vars.insert("{circle}", cl->get("title"));


        Team *team_a = new Team(object->get("team_a").toInt(), keeper);
        vars.insert("{team_a}", team_a->get("title"));

        Team *team_b = new Team(object->get("team_b").toInt(), keeper);
        vars.insert("{team_b}", team_b->get("title"));

        TQueryMap opt;
        opt.insert("competition", keeper->prepareParam(Equal, c->get("id")));
        QList<QVariant> weightId = keeper->getFieldList(OCompetitionWeight,
                                                       "weight", opt);

        opt.clear();
        opt.insert("id", keeper->prepareParam(InSet, weightId));
        opt.insert("order", QStringList() << "title asc");
        QList<QVariant> weights = keeper->getListOfFields(OWeight,
                                                       QStringList() << "title" << "id", opt);
        // int j = 0;
        QStringList rows;
        int team1_win = 0;
        int team2_win = 0;


        for( int i = 0; i < weights.size(); i++ )
        {
            QStringList row = weights.at(i).toStringList();


            opt.clear();
            opt.insert("competition", keeper->prepareParam(Equal,     c->get("id")));
            opt.insert("weight",      keeper->prepareParam(And|Equal, row.at(1)));
            opt.insert("style",      keeper->prepareParam(And|Equal, object->get("style")));
            opt.insert("match",      keeper->prepareParam(And|Equal, object->get("id")));
            opt.insert("order", QStringList() << "num asc" );

            Fight *f = new Fight(opt, keeper);

            Wrestler *red = new Wrestler( f->get("red").toInt(), keeper );
            Rang *rr = new Rang(red->get("rang").toInt(), keeper);

            Wrestler *blue = new Wrestler( f->get("blue").toInt(), keeper );
            Rang *rb = new Rang(blue->get("rang").toInt(), keeper);

            int red_tech = 0;
            int red_class = 0;
            int blue_tech = 0;
            int blue_class = 0;

            int winner = f->get("winner").toInt();
            if ( winner != 0 )
            {
                Classify *cl = new Classify(f->get("classify").toInt(), keeper);
                int win = cl->get("win").toInt();
                int fail = cl->get("fail").toInt();
                if ( red->get("id").toInt() == winner ) {
//                    winnerTitle = team_a->get("title");
                    team1_win++;
                    red_class += win;
                    blue_class += fail;
                } else {
//                    winnerTitle = team_b->get("title");
                    team2_win++;
                    blue_class += win;
                    red_class += fail;
                }

                QStringList redScore = Object::toList(f->get("red_score"));
                QStringList blueScore = Object::toList(f->get("blue_score"));

                red_tech = redScore.at(0).toInt() + redScore.at(1).toInt() + redScore.at(2).toInt();
                blue_tech = blueScore.at(0).toInt() + blueScore.at(1).toInt() + blueScore.at(2).toInt();

            }

            rows << "<tr>";
//            rows << QString("<td class='b_lbr'>%1</td>").arg( ++j );
            rows << QString("<td class='b_lbr'>%1</td>").arg( f->get("num") );
            rows << QString("<td class='b_br'>%1</td>").arg( row.at(0) );


            rows << QString("<td class='b_br'>%1</td>").arg( red->get("title") );
            if ( reportLang.compare("ru") == 0 ) {
                rows << QString("<td class='b_br'>%1</td>").arg( red->get("birthdate") );
                rows << QString("<td class='b_br'>%1</td>").arg( rr->get("title") );
            }
            rows << QString("<td class='b_br'>%1</td>").arg( red_tech  == 0 && winner == 0 ? "" : QString::number(red_tech));
            rows << QString("<td class='b_br'>%1</td>").arg( red_class == 0 && winner == 0 ? "" : QString::number(red_class) );
            rows << QString("<td class='b_br'>%1</td>").arg( "" );
            rows << QString("<td class='b_br'>%1</td>").arg( "" );
            rows << QString("<td class='b_br'>%1</td>").arg( blue_class == 0 && winner == 0 ? "" : QString::number(blue_class) );
            rows << QString("<td class='b_br'>%1</td>").arg( blue_tech == 0 && winner == 0 ? "" : QString::number(blue_tech) );
            rows << QString("<td class='b_br'>%1</td>").arg( blue->get("title") );
            if ( reportLang.compare("ru") == 0 ) {
                rows << QString("<td class='b_br'>%1</td>").arg( blue->get("birthdate") );
                rows << QString("<td class='b_br'>%1</td>").arg( rb->get("title") );
            }
            rows << "</tr>";
        }

        vars.insert("{rows}", rows.join("\n") );


        QString winnerTitle;
        if ( object->get("winner").toInt() != 0 ) {
            Object *team = new Team(object->get("winner").toInt(), keeper);
            winnerTitle = team->get("title");
        }

        // TODO - fill match results
        vars.insert("{match.result}", QString("%1:%2").arg(team1_win).arg(team2_win) );
        vars.insert("{match.winner}", winnerTitle );
        vars.insert("{match.stop}", "" );

        QString sql = QString ("\
                select \
                       r.title, \
                       p.name, \
                       rc.title \
                from \
                       competition_referee as c, \
                       referee as r, \
                       position as p, \
                       rang as rc \
                where \
                       c.competition = ? and \
                       r.id = c.referee and \
                       p.id = c.position and \
                       rc.id = r.rang \
                order \
                       by c.id");

        QSqlQuery query(keeper->database());
        query.prepare(sql);
        query.addBindValue(c->get("id"));

        if ( !query.exec() ) {
            QMessageBox::critical(this, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
            return QString();
        }

        if ( query.size() == 0 ) {
            vars.insert("{referee.chief.category}", "" );
            vars.insert("{referee.chief.title}", "" );
            vars.insert("{secretary.chief.category}", "" );
            vars.insert("{secretary.chief.title}", "" );
        } else {
            while ( query.next() ) {
                QString rtitle = query.value(0).toString();
                QString cname = query.value(2).toString();
                QString pname = query.value(1).toString();
                if ( pname.compare("referee.chief") == 0 ) {
                    vars.insert("{referee.chief.category}", cname );
                    vars.insert("{referee.chief.title}", rtitle );
                } else if ( pname.compare("secretary.chief") == 0 ) {
                    vars.insert("{secretary.chief.category}", cname );
                    vars.insert("{secretary.chief.title}", rtitle );
                }
            }
        }

        return applyTemplateVars(loadTemplate(reportFile), vars);
    }
}
