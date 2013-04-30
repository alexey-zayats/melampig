#include "teammatchprotocol.h"

#include <orm.h>
#include <QtGui>
#include <QtWebKit>
#include <QtSql>

namespace Melampig
{
    TeamMatchProtocol::TeamMatchProtocol(Object *object, Keeper *keeper, QWidget *parent) :
        ReportWidget(object, keeper, parent)
    {
        setObjectName(QString("TeamMatchProtocol:%1:%2").arg(object->objectName()).arg(object->get("id")));

//        competitionCombo->setEnabled(false);

        typeLabel = new QLabel( tr("Type") );
        typeCombo = new QComboBox(filterWidget);
        typeCombo->addItem( tr("Select report type"), 0);
        typeCombo->addItem( tr("Euro"), 1);
        typeCombo->addItem( tr("Russian"), 2);

        matLabel = new QLabel( tr("Mat") );
        matCombo = new QComboBox(filterWidget);
        matCombo->addItem( tr("Select mat"), 0);

        TQueryMap opts;
        QList<QVariant> list = keeper->getListOfFields(OMat, QStringList() << "title" << "id", opts );
        QStringList item;
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            matCombo->addItem( item.at(0), item.at(1) );
        }

        roundLabel = new QLabel( tr("Round") );
        roundCombo = new QComboBox(filterWidget);
        roundCombo->addItem( tr("Select round"), 0);

        list = keeper->getListOfFields(OCRound, QStringList() << "title" << "id", opts );
        for (int i = 0; i < list.size(); i++ ) {
            item = list.at(i).toStringList();
            roundCombo->addItem( item.at(0), item.at(1) );
        }

        filerLayout->addWidget(typeLabel, 3, 0);
        filerLayout->addWidget(typeCombo, 3, 1);
        filerLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 3, 2);

        filerLayout->addWidget(matLabel, 4, 0);
        filerLayout->addWidget(matCombo, 4, 1);
        filerLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 4, 2);

        filerLayout->addWidget(roundLabel, 5, 0);
        filerLayout->addWidget(roundCombo, 5, 1);
        filerLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 5, 2);
    }

    QString TeamMatchProtocol::generateData()
    {
        int type = typeCombo->itemData( typeCombo->currentIndex() ).toInt();
        int mat = matCombo->itemData( matCombo->currentIndex() ).toInt();
        int round = roundCombo->itemData( roundCombo->currentIndex() ).toInt();
        int wid = weightCombo->itemData( weightCombo->currentIndex() ).toInt();
        int sid = styleCombo->itemData( styleCombo->currentIndex() ).toInt();
        int cid = competitionCombo->itemData( competitionCombo->currentIndex() ).toInt();

        QString protoFile;
        if ( type == 1 )
            protoFile = "euro.html";
        else if ( type == 2 )
            protoFile = "russian.html";

        setReportFile(protoFile);
        setReportPath("/../share/reports/startproto");

        object->restore(cid);

        Weight *w = new Weight(wid, keeper);
        Geo *g = new Geo( object->get("geo").toInt(), keeper);
        Mat *m = new Mat(mat, keeper);
        CRound *cr = new CRound(round, keeper);

        QDate start = QDate::fromString(object->get("start"), QString("yyyy-MM-dd"));
        QDate stop = QDate::fromString(object->get("stop"), QString("yyyy-MM-dd"));

        QString date;
        if ( start.month() == stop.month() ) {
            date = QString("%1-%2/%3/%4").arg(start.day()).arg(stop.day()).arg(start.month()).arg(start.year());
        } else {
            date = start.toString("dd/MM/yyyy") + " - " + stop.toString("dd/MM/yyyy");
        }

        vars.insert("{competition}", object->get("title"));
        vars.insert("{weight}", w->get("title"));
        vars.insert("{geo}", g->get("title"));
        vars.insert("{mat}", m->get("title"));
        vars.insert("{date}", date);
        vars.insert("{round}", cr->get("title"));

        delete w;
        delete g;
        delete m;
        delete cr;

        QString sql = "SELECT \
                cw1.sorder as red_num, \
                w1.title as red_name, \
                g1.title as red_geo, \
                r1.title as red_rang, \
                f.num as fight_num, \
                cw2.sorder as blue_num, \
                w2.title as blue_name, \
                g2.title as blue_geo, \
                r2.title as blue_rang \
            FROM \
                fight as f \
                left join competition_wrestler as cw1 on cw1.wrestler = f.red AND cw1.competition = f.competition and cw1.weight = f.weight and cw1.style = f.style \
                left join competition_wrestler as cw2 on cw1.wrestler = f.blue AND cw2.competition = f.competition and cw2.weight = f.weight and cw2.style = f.style \
                left join wrestler as w1 ON w1.id = f.red \
                left join wrestler as w2 on w2.id = f.blue \
                left join geo as g1 on g1.id = w1.geo \
                left join geo as g2 on g2.id = w2.geo \
                left join rang as r1 on r1.id = w1.rang \
                left join rang as r2 on r2.id = w2.rang \
            WHERE \
                f.competition = ? AND \
                f.style = ? AND \
                f.weight = ? AND \
                f.cround = ? AND  \
                f.mat = ? \
            ORDER BY \
                f.num";

        QSqlQuery query(keeper->database());
        query.prepare(sql);

        query.addBindValue(cid);
        query.addBindValue(sid);
        query.addBindValue(wid);
        query.addBindValue(round);
        query.addBindValue(mat);

        if (!query.exec()) {
            QMessageBox::critical(this, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
            return QString();
        }

        QStringList rows;

        if ( type == 1 )
        {
            while( query.next() ) {
                rows << "<tr>";
                rows << QString("<td class='b_lbr'>%1</td>").arg( query.value(0).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(1).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(2).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(3).toString() );
                rows << QString("<td>&nbsp;</td>");
                rows << QString("<td class='b_lbr'>%1</td>").arg( query.value(4).toString() );
                rows << QString("<td>&nbsp;<br/></td>");
                rows << QString("<td class='b_lbr'>%1</td>").arg( query.value(5).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(6).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(7).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(8).toString() );
                rows << QString("<td class='b_br'>&nbsp;<br/></td>");
                rows << "</tr>";
            }
        } else if ( type == 2 ) {
            while( query.next() ) {
                rows << "<tr>";
                rows << QString("<td class='b_lbr' rowspan='2'>%1</td>").arg( query.value(4).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(0).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(1).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(2).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(3).toString() );
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << "</tr>";
                rows << "<tr>";
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(5).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(6).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(7).toString() );
                rows << QString("<td class='b_br'>%1</td>").arg( query.value(8).toString() );
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << QString("<td class='b_br'>&nbsp;</td>");
                rows << "</tr>";
            }
        }

        vars.insert("{rows}", rows.join("\n"));

        return applyTemplateVars(loadTemplate(reportFile), vars);
    }
}
