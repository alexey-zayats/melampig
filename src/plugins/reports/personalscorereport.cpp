#include "personalscorereport.h"

#include <QtSql>
#include <QtGui>
#include <orm.h>

namespace Melampig
{

    PersonalScoreReport::PersonalScoreReport(Object *o, Keeper *k)
        : ReportBase(o,k)
    {
    }


    void PersonalScoreReport::genCompetition(int /*cid*/, int sid, int wid)
    {
        Weight *w = new Weight(wid, keeper);
        Style *s = new Style(sid, keeper);

        pVars.insert("{competition}", comp->get("title"));
        pVars.insert("{weight}", w->get("title"));
        pVars.insert("{style}",  s->get("name"));

        TQueryMap opts;
        opts.insert("competition", keeper->prepareParam(Equal, comp->get("id")));
        opts.insert("weight", keeper->prepareParam(And|Equal, wid));
        opts.insert("style", keeper->prepareParam(And|Equal, sid));
        opts.insert("order", QStringList() << "sorder asc" );

        QList<QVariant> cwList = keeper->getListOfFields(OCompetitionWrestler, QStringList() << "wrestler" << "sorder", opts);

        int wrestlerCount = cwList.size();

        opts.clear();
        opts.insert("num", keeper->prepareParam(Less, wrestlerCount));
        opts.insert("order", QStringList() << "num desc" );
        opts.insert("limit", 1);

        // id, num
        CTour *ct = new CTour(opts, keeper);
        int tourNum = ct->get("num").toInt();
        delete ct;

        QString fightQualify_tmpl    = loadTemplate("fight-qualify.html");
        QString fightRound_tmpl      = loadTemplate("fight-round.html");
        QString fightRoundLast_tmpl  = loadTemplate("fight-round-last.html");
        QString final_tmpl   = loadTemplate("final.html");


        int offset_num = 0;
        if ( tourNum == wrestlerCount )
            offset_num = 0;
        else
            offset_num = (tourNum*2) - wrestlerCount;

        opts.clear();
        opts.insert("num", keeper->prepareParam(Less, wrestlerCount));
        opts.insert("order", QStringList() << "num desc" );

        QList<Object*> tl = keeper->getObjectList(OCTour, opts);
        QList<Object*> tourList;
        for( int i = 0; i < tl.size(); i++ ) {
            Object *o = tl.at(i);
            if (o->get("num").toInt() > 0 ) {
                tourList.append(o);
            }
        }

        QList<QVariant> wrestlerIdList;

        QHash<QString,QString> wrestlerPosIdMap;
        QHash<QString,QString> wrestlerIdPosMap;

        for(int i = 0; i < cwList.size(); i++ ) {
            QStringList list = cwList.at(i).toStringList();

            wrestlerIdList.append( list.at(0) );
            wrestlerPosIdMap.insert( list.at(1), list.at(0) );
            wrestlerIdPosMap.insert( list.at(0), list.at(1) );
        }

        QList<Object*> wrestlerList;

        QHash<QString,Object*> wrestlerIdMap;
        QHash<QString,Object*> wrestlerNumMap;

        QHash<QString,Object*> geoMap;

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, wrestlerIdList));
        opts.insert("order", QStringList() << "id asc" );

        wrestlerList = keeper->getObjectList(OWrestler, opts);

        QList<QVariant> geoIdList;

        for( int z = 0; z < wrestlerList.size(); z++ ) {
            Object *obj = wrestlerList.at(z);
            wrestlerIdMap.insert(obj->get("id"), obj);
            wrestlerNumMap.insert( wrestlerPosIdMap[obj->get("id")], obj);
            if ( obj->get("geo").toInt() )
                geoIdList.append(obj->get("geo"));
        }

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, geoIdList));
        opts.insert("order", QStringList() << "id asc" );

        geoMap = keeper->getObjectMap(OGeo, "id", opts);

        opts.clear();
        opts.insert("competition", keeper->prepareParam(Equal, comp->get("id")));
        opts.insert("weight", keeper->prepareParam(And|Equal, wid));
        opts.insert("style", keeper->prepareParam(And|Equal, sid));
        opts.insert("order", QStringList() << "id asc" );

        QHash<QString,Object*> fightsMap = keeper->getObjectMap(OFight, "order", opts);

        opts.clear();
        opts.insert("order", QStringList() << "id asc" );
        QHash<QString,Object*> classifyMap = keeper->getObjectMap(OClassify, "id", opts);

        switch (tourNum) {
            case 4:  page_tmpl = loadTemplate("page4.html"); break;
            case 8:  page_tmpl = loadTemplate("page8.html"); break;
            case 16: page_tmpl = loadTemplate("page16.html"); break;
            case 32: page_tmpl = loadTemplate("page32.html"); break;
            case 64: page_tmpl = loadTemplate("page64.html"); break;
            default: break;
        }

        QMap<QString,QString> tVars;
        QStringList rows;

        // qualification
        int fightNum = 0;
        int j = 0;
        if ( wrestlerCount < (tourNum*2) )
        {
            int wpos = 0;
            int fnum = tourList.at(j++)->get("num").toInt();
            for( int i = 0; i < fnum; i++ )
            {
                if ( offset_num > wpos++ )
                {
                    QStringList clist = cwList.at(i).toStringList();

                    Object *wrestler = wrestlerIdMap[ clist.at(0) ];
                    Object *geo = geoMap[ wrestler->get("geo") ];

                    tVars.insert("{red.num}", wrestlerIdPosMap[clist.at(0)]);
                    tVars.insert("{red.classify}", "");
                    tVars.insert("{red.fio}", wrestler->get("title"));
                    tVars.insert("{red.geo}", geo->get("title"));
                    tVars.insert("{red.sum}", "");
                    tVars.insert("{red.p1}", "");
                    tVars.insert("{red.p2}", "");
                    tVars.insert("{red.p3}", "");

                    tVars.insert("{blue.num}", "");
                    tVars.insert("{blue.classify}", "");
                    tVars.insert("{blue.fio}", "");
                    tVars.insert("{blue.geo}", "");
                    tVars.insert("{blue.sum}", "");
                    tVars.insert("{blue.p1}", "");
                    tVars.insert("{blue.p2}", "");
                    tVars.insert("{blue.p3}", "");
                } else {
                    fightNum++;

                    Object *fight = fightsMap[ QString::number(fightNum) ];
                    QString redClassify("0");
                    QString blueClassify("0");

                    if ( fight->get("classify").toInt() > 0 )
                    {
                        Object *classify = classifyMap[fight->get("classify")];
                        if ( fight->get("winner").compare( fight->get("red") ) == 0 ) {
                            redClassify = classify->get("win");
                            blueClassify = classify->get("fail");
                        } else {
                            redClassify = classify->get("fail");
                            blueClassify = classify->get("win");
                        }
                    }

                    QStringList redScore = Object::toList(fight->get("red_score"));
                    QStringList blueScore = Object::toList(fight->get("blue_score"));

                    QString redScoreSum = QString::number(redScore.at(0).toInt() + redScore.at(1).toInt() + redScore.at(2).toInt());
                    QString blueScoreSum = QString::number(blueScore.at(0).toInt() + blueScore.at(1).toInt() + blueScore.at(2).toInt());

                    if ( wrestlerIdMap.contains( fight->get("red") ) )
                    {
                        Object *wrestler = wrestlerIdMap[ fight->get("red") ];
                        Object *geo = geoMap[ wrestler->get("geo") ];

                        tVars.insert("{red.num}", wrestlerIdPosMap[ wrestler->get("id") ]);
                        tVars.insert("{red.classify}", redClassify);
                        tVars.insert("{red.fio}", wrestler->get("title"));
                        tVars.insert("{red.geo}", geo->get("title"));
                        tVars.insert("{red.sum}", redScoreSum);
                        tVars.insert("{red.p1}", redScore.at(0));
                        tVars.insert("{red.p2}", redScore.at(1));
                        tVars.insert("{red.p3}", redScore.at(2));
                    }

                    if ( wrestlerIdMap.contains( fight->get("blue") ))
                    {
                        Object *wrestler = wrestlerIdMap[ fight->get("blue") ];
                        Object *geo = geoMap[ wrestler->get("geo") ];

                        tVars.insert("{blue.num}", wrestlerIdPosMap[ wrestler->get("id") ]);
                        tVars.insert("{blue.classify}", blueClassify);
                        tVars.insert("{blue.fio}", wrestler->get("title"));
                        tVars.insert("{blue.geo}", geo->get("title"));
                        tVars.insert("{blue.sum}", blueScoreSum);
                        tVars.insert("{blue.p1}", blueScore.at(0));
                        tVars.insert("{blue.p2}", blueScore.at(1));
                        tVars.insert("{blue.p3}", blueScore.at(2));
                    }
                }
                rows << applyTemplateVars(fightQualify_tmpl, tVars);
                tVars.clear();
                if ( tourNum == 64 ) {
                    if ( i == 15 ) {
                        pVars.insert( QString("{fights:%1/4}").arg(fnum), rows.join("\n"));
                        rows.clear();
                    } else if (i == 31) {
                        pVars.insert( QString("{fights:%1/3}").arg(fnum), rows.join("\n"));
                        rows.clear();
                    } else if (i == 47) {
                        pVars.insert( QString("{fights:%1/2}").arg(fnum), rows.join("\n"));
                        rows.clear();
                    } else if (i == 63) {
                        pVars.insert( QString("{fights:%1/1}").arg(fnum), rows.join("\n"));
                        rows.clear();
                    }
                } else if ( tourNum == 32 && i == (fnum/2)-1 ) {
                    pVars.insert( QString("{fights:%1/2}").arg(fnum), rows.join("\n"));
                    rows.clear();
                }
            }
            pVars.insert( QString("{fights:%1}").arg(fnum), rows.join("\n"));
            rows.clear();
        }

        for(; j<tourList.size(); j++ )
        {
            int level = 0;
            bool last = false;
            QStringList lastData;
            switch (tourNum) {
                case 4:  level = j; break;
                case 8:  {
                    if ( j < 3 ) level = j;
                    else level = j - 2;
                    if ( j == 2 ) last = true;
                    break;
                }
                default:  {
                    if ( j < 4 ) level = j;
                    else level = j - 3;
                    if ( j == 3 ) last = true;
                    break;
                }
            }

            int fnum = tourList.at(j)->get("num").toInt();
            for( int i = 0; i < fnum; i++ )
            {

                fightNum++;
                QString t = QString::number(fightNum);
                if ( !fightsMap.contains(t) )
                {
                    tVars.insert("{level}", QString::number(level));

                    tVars.insert("{red.num}", "");
                    tVars.insert("{red.classify}", "");
                    tVars.insert("{red.fio}", "");
                    tVars.insert("{red.geo}", "");
                    tVars.insert("{red.sum}", "");
                    tVars.insert("{red.p1}", "");
                    tVars.insert("{red.p2}", "");
                    tVars.insert("{red.p3}", "");

                    tVars.insert("{blue.num}", "");
                    tVars.insert("{blue.classify}", "");
                    tVars.insert("{blue.fio}", "");
                    tVars.insert("{blue.geo}", "");
                    tVars.insert("{blue.sum}", "");
                    tVars.insert("{blue.p1}", "");
                    tVars.insert("{blue.p2}", "");
                    tVars.insert("{blue.p3}", "");
                    rows << applyTemplateVars( j == 0 ? fightQualify_tmpl : last ? fightRoundLast_tmpl : fightRound_tmpl, tVars);
                    tVars.clear();
                }
                else
                {
                    Object *fight = fightsMap[ QString::number(fightNum) ];

                    QString redClassify("0");
                    QString blueClassify("0");

                    if ( fight->get("classify").toInt() > 0 )
                    {
                        Object *classify = classifyMap[fight->get("classify")];
                        if ( fight->get("winner").compare( fight->get("red") ) == 0 ) {
                            redClassify = classify->get("win");
                            blueClassify = classify->get("fail");
                        } else {
                            redClassify = classify->get("fail");
                            blueClassify = classify->get("win");
                        }
                    }


                    QStringList redScore = Object::toList(fight->get("red_score"));
                    QStringList blueScore = Object::toList(fight->get("blue_score"));

                    QString redScoreSum = QString::number(redScore.at(0).toInt() + redScore.at(1).toInt() + redScore.at(2).toInt());
                    QString blueScoreSum = QString::number(blueScore.at(0).toInt() + blueScore.at(1).toInt() + blueScore.at(2).toInt());

                    tVars.insert("{level}", QString::number(level));

                    if ( wrestlerIdMap.contains(fight->get("red")) )
                    {
                        Object *wrestler = wrestlerIdMap[ fight->get("red") ];
                        Object *geo = geoMap[ wrestler->get("geo") ];

                        tVars.insert("{red.num}", wrestlerIdPosMap[ wrestler->get("id") ]);
                        tVars.insert("{red.classify}", redClassify);
                        tVars.insert("{red.fio}", wrestler->get("title"));
                        tVars.insert("{red.geo}", geo->get("title"));
                        tVars.insert("{red.sum}", redScoreSum);
                        tVars.insert("{red.p1}", redScore.at(0));
                        tVars.insert("{red.p2}", redScore.at(1));
                        tVars.insert("{red.p3}", redScore.at(2));
                    } else {
                        tVars.insert("{red.num}", "");
                        tVars.insert("{red.classify}", "");
                        tVars.insert("{red.fio}", "");
                        tVars.insert("{red.geo}", "");
                        tVars.insert("{red.sum}", "");
                        tVars.insert("{red.p1}", "");
                        tVars.insert("{red.p2}", "");
                        tVars.insert("{red.p3}", "");
                        tVars.clear();
                    }

                    tVars.insert("{level}", QString::number(level));
                    if ( wrestlerIdMap.contains(fight->get("blue")) )
                    {
                        Object *wrestler = wrestlerIdMap[ fight->get("blue") ];
                        Object *geo = geoMap[ wrestler->get("geo") ];

                        tVars.insert("{blue.num}", wrestlerIdPosMap[ wrestler->get("id") ]);
                        tVars.insert("{blue.classify}", blueClassify);
                        tVars.insert("{blue.fio}", wrestler->get("title"));
                        tVars.insert("{blue.geo}", geo->get("title"));
                        tVars.insert("{blue.sum}", blueScoreSum);
                        tVars.insert("{blue.p1}", blueScore.at(0));
                        tVars.insert("{blue.p2}", blueScore.at(1));
                        tVars.insert("{blue.p3}", blueScore.at(2));
                    } else {
                        tVars.insert("{blue.num}", "");
                        tVars.insert("{blue.classify}", "");
                        tVars.insert("{blue.fio}", "");
                        tVars.insert("{blue.geo}", "");
                        tVars.insert("{blue.sum}", "");
                        tVars.insert("{blue.p1}", "");
                        tVars.insert("{blue.p2}", "");
                        tVars.insert("{blue.p3}", "");
                    }


                    if ( j == (tourList.size()-1) )
                    {
                        Object *wrestler = wrestlerIdMap[ fight->get("winner") ];
                        Object *geo = geoMap[ wrestler->get("geo") ];

                        tVars.insert("{winner.num}", wrestlerIdPosMap[ wrestler->get("id") ]);
                        tVars.insert("{winner.fio}", wrestler->get("title"));
                        tVars.insert("{winner.geo}", geo->get("title"));

                        rows << applyTemplateVars(final_tmpl, tVars);
                        tVars.clear();
                    } else {

                        if ( last ) {
                            lastData << applyTemplateVars( fightRoundLast_tmpl, tVars);
                            tVars.insert("{level}", QString::number(0));
                            rows << applyTemplateVars( fightQualify_tmpl, tVars);
                        }else {
                            rows << applyTemplateVars( j == 0 ? fightQualify_tmpl : fightRound_tmpl, tVars);
                        }

                        tVars.clear();
                    }
                }

                if ( tourNum == 64 ) {
                    if (i == (fnum/4)-1) {
                        pVars.insert( QString("{fights:%1/4}").arg(fnum), rows.join("\n"));

                        QString k = QString("{fights:%1}").arg(fnum);
                        QString a = pVars.contains(k) ? pVars[k] : QString("");
                        pVars.insert( k, a + rows.join("\n"));

                        rows.clear();

                        if ( !lastData.isEmpty()) {
                            pVars.insert( QString("{fights-last:%1/4}").arg(fnum), lastData.join("\n"));
                            lastData.clear();
                        }
                    }
                    if (i == (fnum/2)-1) {
                        pVars.insert( QString("{fights:%1/3}").arg(fnum), rows.join("\n"));

                        QString k = QString("{fights:%1}").arg(fnum);
                        QString a = pVars.contains(k) ? pVars[k] : QString("");
                        pVars.insert( k, a + rows.join("\n"));

                        rows.clear();

                        if ( !lastData.isEmpty()) {
                            pVars.insert( QString("{fights-last:%1/3}").arg(fnum), lastData.join("\n"));
                            lastData.clear();
                        }
                    }
                    if (i == ((3*fnum)/4)-1) {
                        pVars.insert( QString("{fights:%1/2}").arg(fnum), rows.join("\n"));

                        QString k = QString("{fights:%1}").arg(fnum);
                        QString a = pVars.contains(k) ? pVars[k] : QString("");
                        pVars.insert( k, a + rows.join("\n"));
                        rows.clear();

                        if ( !lastData.isEmpty()) {
                            pVars.insert( QString("{fights-last:%1/2}").arg(fnum), lastData.join("\n"));
                            lastData.clear();
                        }
                    }
                    if (i == (fnum-1) ) {
                        pVars.insert( QString("{fights:%1/1}").arg(fnum), rows.join("\n"));

                        QString k = QString("{fights:%1}").arg(fnum);
                        QString a = pVars.contains(k) ? pVars[k] : QString("");
                        pVars.insert( k, a + rows.join("\n"));

                        rows.clear();

                        if ( !lastData.isEmpty()) {
                            pVars.insert( QString("{fights-last:%1/1}").arg(fnum), lastData.join("\n"));
                            lastData.clear();
                        }
                    }
                }
                if ( tourNum == 32 ) {
                    if( i == (fnum/2)-1 ) {
                        pVars.insert( QString("{fights:%1/2}").arg(fnum), rows.join("\n"));

                        QString k = QString("{fights:%1}").arg(fnum);
                        QString a = pVars.contains(k) ? pVars[k] : QString("");
                        if ( last || fnum == 2 ) {
                            pVars.insert( k, a + rows.join("\n"));
                        }
                        rows.clear();

                        if ( !lastData.isEmpty()) {
                            pVars.insert( QString("{fights-last:%1/2}").arg(fnum), lastData.join("\n"));
                            lastData.clear();
                        }
                    }
                }
            }

            QString k = QString("{fights:%1}").arg(fnum);
            QString a = pVars.contains(k) ? pVars[k] : QString("");
            pVars.insert( k, a + rows.join("\n"));
            rows.clear();

            if ( !lastData.isEmpty()) {
                pVars.insert( QString("{fights-last:%1}").arg(fnum), lastData.join("\n"));
                lastData.clear();
            }
        }

        qDeleteAll(fightsMap);
        qDeleteAll(geoMap);
        qDeleteAll(tl);
        qDeleteAll(wrestlerList);
    }

    void PersonalScoreReport::genRepechage(int cid, int sid, int wid)
    {
        QString repechageFirst_tmpl  = loadTemplate("repechage-first.html");
        QString repechageLast_tmpl   = loadTemplate("repechage-last.html");
        QString repechageFight_tmpl   = loadTemplate("repechage-fight.html");

        QString repechageFight_small_tmpl  = loadTemplate("repechage-fight-small.html");
        QString repechageFirst_small_tmpl  = loadTemplate("repechage-first-small.html");

        TQueryMap opts;
        QMap<QString,QString> tVars;
        QStringList rows;

        QSqlQuery query(keeper->database());
        query.prepare("SELECT count(wrestler) FROM competition_wrestler WHERE competition = ? AND style = ? AND weight = ?");

        query.addBindValue(cid);
        query.addBindValue(sid);
        query.addBindValue(wid);

        if ( !query.exec() )
            QMessageBox::critical(0, tr("Error"), tr("Could not get wrestlers count\n") + query.executedQuery() );

        query.next();

        int wrestlerCount = query.value(0).toInt();

        opts.clear();
        opts.insert("num", keeper->prepareParam(Less, wrestlerCount));
        opts.insert("order", QStringList() << "num desc" );

        QList<Object*> tl = keeper->getObjectList(OCTour, opts);
        QList<Object*> tourList;
        for( int i = 0; i < tl.size(); i++) {
            Object *o = tl.at(i);
            if (o->get("num").toInt() > 0 ) {
                tourList.append(o);
            }
        }

        opts.clear();
        opts.insert("competition", keeper->prepareParam(Equal, cid));
        opts.insert("weight", keeper->prepareParam(And|Equal, wid));
        opts.insert("style", keeper->prepareParam(And|Equal, sid));
        opts.insert("order", QStringList() << "id asc" );

        QList<QVariant> cwList = keeper->getListOfFields(OCompetitionWrestler, QStringList() << "wrestler" << "sorder", opts);

        QList<QVariant> wrestlerIdList;

        QHash<QString,QString> wrestlerPosIdMap;
        QHash<QString,QString> wrestlerIdPosMap;

        for(int i = 0; i < cwList.size(); i++ ) {
            QStringList list = cwList.at(i).toStringList();
            wrestlerIdList.append( list.at(0) );
            wrestlerPosIdMap.insert( list.at(1), list.at(0) );
            wrestlerIdPosMap.insert( list.at(0), list.at(1) );
        }

        QHash<QString,Object*> wrestlerIdMap;
        QHash<QString,Object*> wrestlerNumMap;
        QHash<QString,Object*> geoMap;
        QList<Object*> wrestlerList;

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, wrestlerIdList));
        opts.insert("order", QStringList() << "id asc" );

        wrestlerList = keeper->getObjectList(OWrestler, opts);

        QList<QVariant> geoIdList;
        for( int z = 0; z < wrestlerList.size(); z++ )
        {
            Object *obj = wrestlerList.at(z);
            wrestlerIdMap.insert(obj->get("id"), obj);
            wrestlerNumMap.insert( wrestlerPosIdMap[obj->get("id")], obj);
            if ( obj->get("geo").toInt() )
                geoIdList.append(obj->get("geo"));
        }

        opts.clear();
        opts.insert("id", keeper->prepareParam(InSet, geoIdList));
        opts.insert("order", QStringList() << "id asc" );

        geoMap = keeper->getObjectMap(OGeo, "id", opts);

        opts.clear();

        QList<QVariant> rrounds;
        rrounds.append("'consolation-1'");
        rrounds.append("'consolation-2'");

        opts.insert("name", keeper->prepareParam(InSet, rrounds));
        opts.insert("order", QStringList() << "id" );

        QList<Object*> roundList = keeper->getObjectList(OCRound, opts);
        rrounds.clear();
        for( int i = 0; i < roundList.size(); i++)
            rrounds.append( roundList.at(i)->get("id") );

        opts.clear();
        opts.insert("competition", keeper->prepareParam(Equal, cid));
        opts.insert("weight", keeper->prepareParam(And|Equal, wid));
        opts.insert("style", keeper->prepareParam(And|Equal, sid));
        opts.insert("cround", keeper->prepareParam(And|InSet, rrounds));
        opts.insert("order", QStringList() << "id asc" );

        QList<Object*> fightList = keeper->getObjectList(OFight, opts);

        QHash<QString,QList<Object*> > fightsMap;
        for ( int i = 0; i < fightList.size(); i++ )
        {
            Object *f = fightList.at(i);
            if ( fightsMap.contains(f->get("cround"))) {
                fightsMap[f->get("cround")].append(f);
            } else {
                QList<Object*> list;
                list.append(f);
                fightsMap.insert(f->get("cround"), list);
            }
        }

        opts.clear();
        opts.insert("order", QStringList() << "id asc" );
        QHash<QString,Object*> classifyMap = keeper->getObjectMap(OClassify, "id", opts);

        rows.clear();

        QList<Object*> reps;
        Object *round = 0;
        Object *fight = 0;

        int repSize = tourList.size() - 2;

        for( int j = 0; j < roundList.size(); j++ )
        {
            rows.clear();
            reps.clear();

            round = roundList.at(j);
            if ( fightsMap.contains(round->get("id"))) {
                reps = fightsMap[round->get("id")];
            }

            int diff = repSize - reps.size();
            for( int i = 0; i < repSize ; i++ )
            {
                fight = ( reps.size() == 0 ) ? 0 :
                           (diff == 0) ? reps.at(i) :
                           (diff == 1 && i > 0) ? reps.at(i-1) : 0;
                if ( fight )
                {
                    QString redClassify("0");
                    QString blueClassify("0");

                    if ( fight->get("classify").toInt() > 0 )
                    {
                        Object *classify = classifyMap[fight->get("classify")];
                        if ( fight->get("winner").compare( fight->get("red") ) == 0 ) {
                            redClassify = classify->get("win");
                            blueClassify = classify->get("fail");
                        } else {
                            redClassify = classify->get("fail");
                            blueClassify = classify->get("win");
                        }
                    }

                    QStringList redScore = Object::toList(fight->get("red_score"));
                    QStringList blueScore = Object::toList(fight->get("blue_score"));

                    QString redScoreSum = QString::number(redScore.at(0).toInt() + redScore.at(1).toInt() + redScore.at(2).toInt());
                    QString blueScoreSum = QString::number(blueScore.at(0).toInt() + blueScore.at(1).toInt() + blueScore.at(2).toInt());

                    Object *red = wrestlerIdMap[ fight->get("red") ];
                    if ( red )
                    {
                       Object *geo = geoMap[ red->get("geo") ];

                       tVars.insert("{red.num}", wrestlerIdPosMap[ red->get("id") ]);
                       tVars.insert("{red.classify}", redClassify);
                       tVars.insert("{red.fio}", red->get("title"));
                       tVars.insert("{red.geo}", geo->get("title"));
                       tVars.insert("{red.sum}", redScoreSum);
                       tVars.insert("{red.p1}", redScore.at(0));
                       tVars.insert("{red.p2}", redScore.at(1));
                       tVars.insert("{red.p3}", redScore.at(2));
                    } else {
                        tVars.insert("{red.num}", "");
                        tVars.insert("{red.classify}", "");
                        tVars.insert("{red.fio}", "");
                        tVars.insert("{red.geo}", "");
                        tVars.insert("{red.sum}", "");
                        tVars.insert("{red.p1}", "");
                        tVars.insert("{red.p2}", "");
                        tVars.insert("{red.p3}", "");
                    }

                    Object *blue = wrestlerIdMap[ fight->get("blue") ];
                    if ( blue )
                    {
                        Object *geo = geoMap[ blue->get("geo") ];

                        tVars.insert("{blue.num}", wrestlerIdPosMap[ blue->get("id")]);
                        tVars.insert("{blue.classify}", blueClassify);
                        tVars.insert("{blue.fio}", blue->get("title"));
                        tVars.insert("{blue.geo}", geo->get("title"));
                        tVars.insert("{blue.sum}", blueScoreSum);
                        tVars.insert("{blue.p1}", blueScore.at(0));
                        tVars.insert("{blue.p2}", blueScore.at(1));
                        tVars.insert("{blue.p3}", blueScore.at(2));
                    } else {
                        tVars.insert("{blue.num}", "");
                        tVars.insert("{blue.classify}", "");
                        tVars.insert("{blue.fio}", "");
                        tVars.insert("{blue.geo}", "");
                        tVars.insert("{blue.sum}", "");
                        tVars.insert("{blue.p1}", "");
                        tVars.insert("{blue.p2}", "");
                        tVars.insert("{blue.p3}", "");
                    }



                } else {
                    tVars.insert("{red.num}", "");
                    tVars.insert("{red.classify}", "");
                    tVars.insert("{red.fio}", "");
                    tVars.insert("{red.geo}", "");
                    tVars.insert("{red.sum}", "");
                    tVars.insert("{red.p1}", "");
                    tVars.insert("{red.p2}", "");
                    tVars.insert("{red.p3}", "");

                    tVars.insert("{blue.num}", "");
                    tVars.insert("{blue.classify}", "");
                    tVars.insert("{blue.fio}", "");
                    tVars.insert("{blue.geo}", "");
                    tVars.insert("{blue.sum}", "");
                    tVars.insert("{blue.p1}", "");
                    tVars.insert("{blue.p2}", "");
                    tVars.insert("{blue.p3}", "");
                }

                tVars.insert("{level}", QString::number(i));

                QString tmpl;
                if ( i == (repSize - 1)) {
                   tmpl = repechageLast_tmpl;

                   fight = reps.size() > 0 ? reps.at(reps.size()-1) : 0;
                   Object *wrestler = fight ? wrestlerIdMap[ fight->get("winner") ] : 0;
                   if ( wrestler )
                   {
                       Object *geo = geoMap[ wrestler->get("geo") ];
                       tVars.insert("{bronze.num}", wrestlerIdPosMap[ wrestler->get("id") ]);
                       tVars.insert("{bronze.fio}", wrestler->get("title"));
                       tVars.insert("{bronze.geo}", geo->get("title"));
                   } else {
                       tVars.insert("{bronze.num}", "");
                       tVars.insert("{bronze.fio}", "");
                       tVars.insert("{bronze.geo}", "");
                   }

                }
                else if ( i > 0 ) {
                    tmpl = repSize > 4 ? repechageFight_small_tmpl : repechageFight_tmpl;
                } else {
                    tmpl = repSize > 4 ? repechageFirst_small_tmpl : repechageFirst_tmpl;
                }


                pVars.insert( QString("{repechage:%1:%2}").arg( j == 0 ? "up" : "low").arg(i+1), applyTemplateVars(tmpl, tVars) );
                tVars.clear();
            }
        }

        qDeleteAll(fightList);
        qDeleteAll(geoMap);
        qDeleteAll(wrestlerList);
    }

    void PersonalScoreReport::genClassement(bool ru, int cid, int sid, int wid)
    {
        // CLASSIFY
        bool show_points = false;

        QMap<QString,QString> tVars, aVars;
        QStringList rows;

        QString sql = QString("\
           select \
              c.sorder, \
              w.title, \
              extract( year from w.birthdate), \
              r.name, \
              g.title, \
              ca.title, \
              c.classify_points, \
              c.vt, \
              c.va, \
              c.vb, \
              c.vf, \
              c.ev, \
              c.ex, \
              c.st, \
              c.sp, \
              c.pp, \
              c.po, \
              c.e2, \
              c.tech_points_win, \
              c.tech_points_loose \
           from \
              wrestler_classement(?, ?, ?) as cc, \
              competition_wrestler as c, \
              wrestler as w \
                left join coach as ca on ca.id = w.coach \
                left join geo as g on g.id = w.geo \
                left join rang as r on r.id = w.rang \
           where \
              c.competition = ? AND \
              c.weight = ? AND \
              c.style = ? AND \
              w.id = c.wrestler AND \
              intset(c.wrestler) && cc \
           order by \
              idx(cc,w.id)");

        QSqlQuery query(keeper->database());
        query.prepare(sql);

        query.addBindValue(cid);
        query.addBindValue(wid);
        query.addBindValue(sid);

        query.addBindValue(cid);
        query.addBindValue(wid);
        query.addBindValue(sid);

        if ( !query.exec() ) {
            QMessageBox::critical(0, tr("Error"), query.lastError().text() );
            return;
        }

        QString classmentHeader = show_points ? loadTemplate("classify-points.html")
                                           : loadTemplate("classify.html");
        QString classmentRow = show_points ? loadTemplate("classify-row-points.html")
                                           : loadTemplate("classify-row.html");

        QString classifyPage = loadTemplate("classify-page.html");

        int onPage = show_points ? 40 : 60;

        int counter = 1;
        while( query.next() )
        {
            tVars.insert("{counter}", QString::number((counter == 4 || counter == 6 ? counter - 1 : counter)));
            tVars.insert("{wrestler.num}", query.value(0).toString());
            tVars.insert("{wrestler.fio}", query.value(1).toString());
            tVars.insert("{wrestler.year}", query.value(2).toString());
            if ( ru )
                tVars.insert("{wrestler.rang}", query.value(3).toString());
            tVars.insert("{wrestler.geo}", query.value(4).toString());
            tVars.insert("{wrestler.coach}", query.value(5).toString());
            if ( show_points )
            {
                tVars.insert("{wrestler.classify_points}", query.value(6).toString());
                tVars.insert("{wrestler.vt}", query.value(7).toString());
                tVars.insert("{wrestler.va}", query.value(8).toString());
                tVars.insert("{wrestler.vb}", query.value(9).toString());
                tVars.insert("{wrestler.vf}", query.value(10).toString());
                tVars.insert("{wrestler.ev}", query.value(11).toString());
                tVars.insert("{wrestler.ex}", query.value(12).toString());
                tVars.insert("{wrestler.st}", query.value(13).toString());
                tVars.insert("{wrestler.sp}", query.value(14).toString());
                tVars.insert("{wrestler.pp}", query.value(15).toString());
                tVars.insert("{wrestler.po}", query.value(16).toString());
                tVars.insert("{wrestler.e2}", query.value(17).toString());
                tVars.insert("{wrestler.tech_points_win}", query.value(18).toString());
                tVars.insert("{wrestler.tech_points_loose}", query.value(19).toString());
            }
            rows << applyTemplateVars(classmentRow, tVars);
            tVars.clear();
            counter++;
            if ( counter == onPage ) {
                tVars.insert("{rows}", rows.join("\n") );
                rows.clear();

                aVars.insert("{classify}", applyTemplateVars(classmentHeader, tVars) );
                tVars.clear();

                QString k("{classify}");
                QString a = pVars.contains(k) ? pVars[k] : QString("");
                pVars.insert(k, a + applyTemplateVars(classifyPage, aVars));
                aVars.clear();
            }
        }

        tVars.insert("{rows}", rows.join("\n") );
        rows.clear();

        aVars.insert("{classify}", applyTemplateVars(classmentHeader, tVars) );
        tVars.clear();

        QString k("{classify}");
        QString a = pVars.contains(k) ? pVars[k] : QString("");
        pVars.insert(k, a + applyTemplateVars(classifyPage, aVars));
        aVars.clear();
    }

    QString PersonalScoreReport::generate(bool ru)
    {
        setReportFile("index.html");
        setReportPath( QString("/../share/reports/%1/wrestler/competition").arg(ru ? "ru" : "eng") );

        QString sql;
        sql += "SELECT s.id, s.title FROM competition_style cs, style as s WHERE cs.competition = ? AND s.id = cs.style ORDER BY s.title";

        QSqlQuery query(keeper->database());
        query.prepare(sql);
        query.addBindValue(comp->get("id"));

        QStringList pages;

        if (!query.exec()) {
            QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
        } else {

            sql.clear();
            sql += "SELECT w.id, w.title FROM competition_weight cw, weight as w WHERE cw.competition = ? AND w.id = cw.weight ORDER BY w.title";

            QSqlQuery nQuery(keeper->database());
            nQuery.prepare(sql);
            nQuery.addBindValue(comp->get("id"));

            while(query.next()) {

                if (!nQuery.exec()) {
                    QMessageBox::critical(0, tr("Error"), query.executedQuery() + "\n" + query.lastError().text() );
                } else {
                    while(nQuery.next()) {
                        qDebug() << "GENERATE SCORESHEET FOR: " << comp->get("id").toInt() << query.value(0).toInt() << nQuery.value(0).toInt();

                        genCompetition(comp->get("id").toInt(), query.value(0).toInt(), nQuery.value(0).toInt());
                        genRepechage(comp->get("id").toInt(), query.value(0).toInt(), nQuery.value(0).toInt());
                        genClassement(ru, comp->get("id").toInt(), query.value(0).toInt(), nQuery.value(0).toInt());

                        vars.insert("{page}", applyTemplateVars(page_tmpl, pVars) );
                        pVars.clear();

                        pages << applyTemplateVars(loadTemplate(reportFile), vars);
                    }
                }

            }
        }

        return pages.join("\n");
    }
}
