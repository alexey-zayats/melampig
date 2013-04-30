#include "xmlreport.h"


#include <QtXml>
#include <QFileDialog>
#include <QMessageBox>
#include <QtSql>

#include <objectselector.h>

namespace Melampig
{
    XmlReport::XmlReport(Keeper *keeper, QWidget *parent) :
        QMainWindow(parent),
        keeper(keeper)
    {
        ui.setupUi(this);

        comp = 0;
        ui.textBrowser->setAcceptRichText(false);
        ui.generateButton->setEnabled(false);
        ui.saveButton->setEnabled(false);

        attr = new Attr("competition", tr("competition"), Attr::ObjectRef, false, true, OCompetition);

        os = new ObjectSelector(attr, keeper, this);
        os->setParentWidget(parent);

        ui.gridLayout->addWidget(os, 0, 1, 1, 2);

        connect(os, SIGNAL(objectSelected(Object*)), this, SLOT(competitionSelected(Object*)));

        connect(ui.fileChoose,        SIGNAL(clicked()), this, SLOT(fileChoose_clicked()));
        connect(ui.generateButton,    SIGNAL(clicked()), this, SLOT(generateButton_clicked()));
        connect(ui.saveButton,        SIGNAL(clicked()), this, SLOT(saveButton_clicked()));
    }

    XmlReport::~XmlReport()
    {
        if (comp) delete comp;
        delete attr;
    }

    void XmlReport::competitionSelected(Object *object)
    {
        comp = object;
        ui.generateButton->setEnabled(true);
    }

    void XmlReport::fileChoose_clicked()
    {
        QString file = QFileDialog::getSaveFileName(this, tr("Select file to store xml report"), QString(), QString("*.xml") );
        ui.fileEdit->setText(file);
        ui.saveButton->setEnabled(true);
    }

    QDomDocument *XmlReport::getReport()
    {
        QDomDocument *doc = new QDomDocument();
        QDomElement root = doc->createElement("turnir");
        doc->appendChild(root);

        QDomElement nametag = doc->createElement("name");
        nametag.appendChild(doc->createTextNode(comp->get("title")));
        root.appendChild(nametag);

        Geo *geo = new Geo(comp->get("geo").toInt(), keeper);
        QDomElement locationTag = doc->createElement("location");
        locationTag.appendChild(doc->createTextNode(geo->get("title")));
        root.appendChild(locationTag);
        delete geo;

        QDate start = QDate::fromString(comp->get("start"), QString("yyyy-MM-dd"));
        QDate stop = QDate::fromString(comp->get("stop"), QString("yyyy-MM-dd"));

        QString date;
        if ( start.month() == stop.month() ) {
            date = QString("%1-%2/%3/%4").arg(start.day()).arg(stop.day()).arg(start.month()).arg(start.year());
        } else {
            date = start.toString("dd/MM/yyyy") + " - " + stop.toString("dd/MM/yyyy");
        }

        QDomElement dateTag = doc->createElement("date");
        dateTag.appendChild(doc->createTextNode(date));
        root.appendChild(dateTag);

        // -----------------


        QString sql;
        sql += "SELECT s.id, s.title FROM competition_style cs, style as s WHERE cs.competition = ? AND s.id = cs.style ORDER BY title";

        QSqlQuery query(keeper->database());
        query.prepare(sql);
        query.addBindValue(comp->get("id"));

        if (!query.exec()) {
            QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
            return doc;
        }

        QHash<QString, QString> styles;
        while(query.next()) {
            styles.insert(query.value(0).toString(),
                        query.value(1).toString());
        }

        query.clear();

        QHash<QString, QString>::Iterator it = styles.begin();
        for( ; it != styles.end(); it++)
        {
            QString styleName = it.value();
            QString styleId = it.key();

            QDomElement styleTag = doc->createElement("style");
            styleTag.setAttribute("name", styleName);

            sql.clear();
            sql += "select wg.alpha3, wg.title, cm.heft, w.title, wh.title \
                    from \
                        competition_wrestler as cm, \
                        wrestler as w, \
                        geo as wg, \
                        weight as wh \
                    where \
                        cm.competition = ? and \
                        cm.style = ? and \
                        w.id = cm.wrestler and \
                        wg.id = w.geo and \
                        wh.id = cm.weight \
                    order by \
                        wg.title;";

            query.prepare(sql);
            query.addBindValue(comp->get("id"));
            query.addBindValue(styleId);

            if (!query.exec()) {
                QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
                return doc;
            }

            QDomElement membersTag = doc->createElement("members");
            while(query.next()) {

                QString countryCode = query.value(0).toString();
                QString countryTitle = query.value(1).toString();

                QDomElement athletTag = doc->createElement("athlet");
                athletTag.setAttribute("weight", query.value(2).toString());
                athletTag.setAttribute("country", countryCode.isEmpty() ? countryTitle : countryCode );
                QDomElement nameTag = doc->createElement("name");
                nameTag.appendChild(doc->createTextNode(query.value(3).toString()));
                athletTag.appendChild(nameTag);
                membersTag.appendChild(athletTag);
            }
            styleTag.appendChild(membersTag);

            sql.clear();
            sql += "select \
                    wh.title, \
                    red.title, f.red_score, rg.alpha3, rg.title, \
                    blue.title, f.blue_score, bg.alpha3, bg.title, \
                    case f.winner \
                        when f.red then red.title \
                        when f.blue then blue.title \
                    end as winner \
                from \
                    fight as f, \
                    wrestler as red, \
                    wrestler as blue, \
                    geo as rg, \
                    geo as bg, \
                    weight as wh \
                where \
                    f.competition = ? and \
                    f.style = ? and \
                    red.id = f.red and \
                    blue.id = f.blue and \
                    wh.id = f.weight and \
                    rg.id = red.geo and \
                    bg.id = blue.geo \
                order by 1;";

            query.prepare(sql);
            query.addBindValue(comp->get("id"));
            query.addBindValue(styleId);

            if (!query.exec()) {
                QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
                return doc;
            }

            while(query.next()) {
                QDomElement duelTag = doc->createElement("duel");
                duelTag.setAttribute("weight", query.value(0).toString());

                QString countryCode = query.value(3).toString();
                QString countryTitle = query.value(4).toString();

                QDomElement redAthletTag = doc->createElement("athlet");
                redAthletTag.setAttribute("country", countryCode.isEmpty() ? countryTitle : countryCode );

                QDomElement redNameTag = doc->createElement("name");
                redNameTag.appendChild(doc->createTextNode(query.value(1).toString()));

                redAthletTag.appendChild(redNameTag);

                QStringList redScore = Object::toList(query.value(2).toString());

                QDomElement rp1 = doc->createElement("p1");
                rp1.appendChild(doc->createTextNode(redScore.at(0)));
                redAthletTag.appendChild(rp1);

                QDomElement rp2 = doc->createElement("p2");
                rp2.appendChild(doc->createTextNode(redScore.at(1)));
                redAthletTag.appendChild(rp2);

                QDomElement rp3 = doc->createElement("p3");
                rp3.appendChild(doc->createTextNode(redScore.at(2)));
                redAthletTag.appendChild(rp3);


                duelTag.appendChild(redAthletTag);

                countryCode = query.value(7).toString();
                countryTitle = query.value(8).toString();

                QDomElement blueAthletTag = doc->createElement("athlet");
                blueAthletTag.setAttribute("country", countryCode.isEmpty() ? countryTitle : countryCode );

                QDomElement blueNameTag = doc->createElement("name");
                blueNameTag.appendChild(doc->createTextNode(query.value(5).toString()));

                blueAthletTag.appendChild(blueNameTag);

                QStringList blueScore = Object::toList(query.value(6).toString());

                QDomElement bp1 = doc->createElement("p1");
                bp1.appendChild( doc->createTextNode(blueScore.at(0)));
                blueAthletTag.appendChild(bp1);

                QDomElement bp2 = doc->createElement("p2");
                bp2.appendChild( doc->createTextNode(blueScore.at(1)));
                blueAthletTag.appendChild(bp2);

                QDomElement bp3 = doc->createElement("p3");
                bp3.appendChild( doc->createTextNode(blueScore.at(2)));
                blueAthletTag.appendChild(bp3);

                duelTag.appendChild(blueAthletTag);

                QDomElement winnerTag = doc->createElement("winner");
                winnerTag.appendChild( doc->createTextNode(query.value(9).toString()) );
                duelTag.appendChild(winnerTag);

                styleTag.appendChild(duelTag);
            }

            root.appendChild(styleTag);
        }
        return doc;
    }

    void XmlReport::generateButton_clicked() {
        QDomDocument *doc = this->getReport();

        QDomNode xmlNode = doc->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc->insertBefore(xmlNode, doc->firstChild());

        ui.textBrowser->setPlainText( doc->toString(4) );
        delete doc;
    }

    void XmlReport::saveButton_clicked()
    {
        QString fileName = ui.fileEdit->text();
        if (fileName.isEmpty())  {
            QMessageBox::warning(this, tr("Select file"), tr("Please, select file to store xml report"));
            return;
        }

        QFile file(ui.fileEdit->text());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QDomDocument *doc = this->getReport();

        QTextStream out(&file);

        out.setCodec( QTextCodec::codecForName("UTF-8") );
        doc->save(out, 4, QDomNode::EncodingFromTextStream );

        file.close();
        delete doc;

        QMessageBox::information(this, tr("Report stored"), tr("Report successfully stored"));
    }

}
