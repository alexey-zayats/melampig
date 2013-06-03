#include "fightcontrol-db.h"
#include "ui_fightcontrol-db.h"

#include "fightdisplayfila-db.h"

#include "fighttable-db.h"
#include "winnerdialog-db.h"

#include <QtGui>
#include <QSvgWidget>
#include <QFileDialog>
#include <QTimer>
#include <QSettings>
#include <QLineEdit>
#include <phonon>

namespace Melampig
{

    FightControl::FightControl(Keeper *keeper, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FightControlDB),
        keeper(keeper)
    {
        ui->setupUi(this);

        isGR_18052013 = false;
//        display = new FightDisplay();
        display = new FightDisplayFila();
        display->showSplash();
        display->show();

        connect(this, SIGNAL(settingsChanged()), display, SLOT(changeSettings()));

        TQueryMap opts;
        QList<QVariant> list = keeper->getListOfFields(OAction, QStringList() << "name" << "id", opts);
        for( int i = 0; i < list.size(); i++ ) {
            QStringList row = list.at(i).toStringList();
            actions.insert(row.at(0), row.at(1).toInt());
        }

        list = keeper->getListOfFields(OFRound, QStringList() << "name" << "id" << "title", opts);
        for( int i = 0; i < list.size(); i++ ) {
            QStringList row = list.at(i).toStringList();
            rounds.insert(row.takeFirst(), row);
        }

        ui->redFlagLayout->removeWidget(ui->redFlagLabel);
        ui->redFlagLabel->deleteLater();

        redFlagWidget = new QSvgWidget(this);
        redFlagWidget->setMinimumHeight(100);
        ui->redFlagLayout->addWidget(redFlagWidget, 0, 0);

        ui->blueFlagLayout->removeWidget(ui->blueFlagLabel);
        ui->blueFlagLabel->deleteLater();

        blueFlagWidget = new QSvgWidget(this);
        blueFlagWidget->setMinimumHeight(100);
        ui->blueFlagLayout->addWidget(blueFlagWidget, 0, 0);

        initButtons();

        countdown = new QTimer();
        countdown->setInterval(1000);
        countdown->setSingleShot(false);
        connect(countdown,SIGNAL(timeout()),this,SLOT(timeOut()));

        initButtonStates();
        initSound();
        initPointManage();

        timerOrder = Descendant;
        lastRound = 0;

        object = 0;
    }

    void FightControl::keyPressEvent ( QKeyEvent * event )
    {
        if ( event->key() == Qt::Key_Escape) {
            emit ui->sndButton1->click();
        }
    }

    void FightControl::initButtonStates()
    {

        ui->startPauseButton->setEnabled(false);
        startMilliseconds = 0;
        stopMilliseconds = 0;

        setTimer();

        ui->splashButton->toggle();
        display->showSplash();

        redFlagWidget->load(QByteArray());
        display->setRedFlag(QByteArray());

        ui->redCountryLabel->clear();
        display->setRedCountry("");

        ui->redWrestlerLabel->clear();
        display->setRedWrestler("");

        ui->redPoint1->clear();
        ui->redPoint2->clear();
        ui->redPoint3->clear();
        ui->redPoint->setText(QString("0"));

        blueFlagWidget->load(QByteArray());
        display->setBlueFlag(QByteArray());

        ui->blueCountryLabel->clear();
        display->setBlueCountry("");

        ui->blueWrestlerLabel->clear();
        display->setBlueWrestler("");

        ui->bluePoint1->clear();
        ui->bluePoint2->clear();
        ui->bluePoint3->clear();
        ui->bluePoint->setText(QString("0"));

        ui->styleLabel->clear();
        ui->weightLabel->clear();
        ui->numLabel->clear();

        QString empty = QString("");
        display->clearFigthResult();

        display->setBluePoint(QString("0"));
        display->setRedPoint(QString("0"));

        display->setStyleLabel( empty );
        display->setNumberLabel( empty );
        display->setRound(empty);

        display->setTimer( QString("0:00") );
        ui->timerLabel->setText( QString("0:00") );

        ui->redBan1->setChecked(false);
        ui->redBan2->setChecked(false);
        ui->redBan3->setChecked(false);

        ui->blueBan1->setChecked(false);
        ui->blueBan2->setChecked(false);
        ui->blueBan3->setChecked(false);

        ui->roundOneButton->setEnabled(true);
        ui->roundOneButton->setChecked(false);

        ui->roundTwoButton->setEnabled(false);
        ui->roundTwoButton->setChecked(false);

        ui->roundThreeButton->setEnabled(false);
        ui->roundThreeButton->setChecked(false);

        ui->round1ovetime->setChecked(false);
        ui->round1ovetime->setEnabled(false);

        ui->round2ovetime->setChecked(false);
        ui->round2ovetime->setEnabled(false);

        ui->round3ovetime->setChecked(false);
        ui->round3ovetime->setEnabled(false);

        ui->breaktimeButton->setChecked(false);
        ui->breaktimeButton->setEnabled(false);

        ui->stopButton->setEnabled(false);
        ui->roundOneButton->setEnabled(false);
        ui->endofmatchButton->setEnabled(false);

        ui->redFirstWin->setChecked(false);
        ui->redSecondWin->setChecked(false);
        ui->redThirdWin->setChecked(false);

        ui->blueFirstWin->setChecked(false);
        ui->blueSecondWin->setChecked(false);
        ui->blueThirdWin->setChecked(false);

        ui->plusRedPoint1->setEnabled(false);
        ui->plusRedPoint2->setEnabled(false);
        ui->plusRedPoint3->setEnabled(false);

        ui->minusRedPoint1->setEnabled(false);
        ui->minusRedPoint2->setEnabled(false);
        ui->minusRedPoint3->setEnabled(false);

        ui->plusBluePoint1->setEnabled(false);
        ui->plusBluePoint2->setEnabled(false);
        ui->plusBluePoint3->setEnabled(false);

        ui->minusBluePoint1->setEnabled(false);
        ui->minusBluePoint2->setEnabled(false);
        ui->minusBluePoint3->setEnabled(false);

        ui->plusRedBan->setEnabled(false);
        ui->minusRedBan->setEnabled(false);

        ui->plusBlueBan->setEnabled(false);
        ui->minusBlueBan->setEnabled(false);

        ui->plus1RedPoint->setEnabled(false);
        ui->plus2RedPoint->setEnabled(false);
        ui->plus3RedPoint->setEnabled(false);
        ui->plus5RedPoint->setEnabled(false);

        ui->plus1BluePoint->setEnabled(false);
        ui->plus2BluePoint->setEnabled(false);
        ui->plus3BluePoint->setEnabled(false);
        ui->plus5BluePoint->setEnabled(false);

        ui->minusRedPoint->setEnabled(false);
        ui->plusRedPoint->setEnabled(false);

        ui->minusBluePoint->setEnabled(false);
        ui->plusBluePoint->setEnabled(false);

        ui->minusTimeButton->setEnabled(false);
        ui->plusTimeButton->setEnabled(false);

        ui->redFirstWin->setEnabled(false);
        ui->redSecondWin->setEnabled(false);
        ui->redThirdWin->setEnabled(false);

        ui->blueFirstWin->setEnabled(false);
        ui->blueSecondWin->setEnabled(false);
        ui->blueThirdWin->setEnabled(false);

        ui->startPauseButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
    }

    FightControl::~FightControl()
    {
        if ( object )
            delete object;
        delete display;
        delete ui;
    }

    void FightControl::do_releaseFight()
    {
        if ( object ) {
//            object->restore(object->get("id").toInt());
            object->set("on_fight", "false");
            object->store();
        }
    }

    void FightControl::selectMatch_clicked()
    {
        do_releaseFight();

        FightTable *table = new FightTable (keeper, this);
        if ( table->exec() != QDialog::Accepted )
            return;

        ui->selectMatch->setEnabled(false);

        int id = table->getFightId();
        object = new Fight(id, keeper);

        red = new Wrestler(object->get("red").toInt(), keeper);
        Geo *g = new Geo(red->get("geo").toInt(), keeper);


        QByteArray data;
        data += g->get("flag");
        if ( !data.isEmpty() ) {
            redFlagWidget->load(QByteArray::fromBase64(data));
            display->setRedFlag(QByteArray::fromBase64(data));
        }

        // country

        QString redCode = g->get("alpha3").toUpper();
        if (redCode.isEmpty()) redCode = g->get("title");

        ui->redCountryLabel->setText(redCode);
        display->setRedCountry(redCode);

        // fio
        ui->redWrestlerLabel->setText( red->get("lastname").toUpper() );
        display->setRedWrestler(red->get("lastname").toUpper());

        delete g;

        // -- BLUE --


        blue = new Wrestler(object->get("blue").toInt(), keeper);
        g = new Geo(blue->get("geo").toInt(), keeper);


        data.clear();
        data += g->get("flag");

        if ( !data.isEmpty() ) {
            blueFlagWidget->load(QByteArray::fromBase64(data));
            display->setBlueFlag(QByteArray::fromBase64(data));
        }
        data.clear();

        // country
        QString blueCode = g->get("alpha3").toUpper();
        if (blueCode.isEmpty()) blueCode = g->get("title");

        ui->blueCountryLabel->setText(blueCode);
        display->setBlueCountry(blueCode);

        // fio
        ui->blueWrestlerLabel->setText( blue->get("lastname").toUpper() );
        display->setBlueWrestler(blue->get("lastname").toUpper());


        // style
        Style *s = new Style( object->get("style").toInt(), keeper );

        isGR_18052013 = s->get("name").compare("GR") == 0;
        isGR_18052013 = true;

        ui->styleLabel->setText(s->get("name"));

        roundnum = s->get("rnum").toInt();
        roundtime = s->get("rtime").toInt();
        overtime = s->get("otime").toInt();
        resttime = s->get("rsttime").toInt();

        overtimedirection = s->get("otdir").compare("true") == 0 ? true : false;
        roundtimedirection = s->get("rtdir").compare("true") == 0 ? true : false;
        resttimedirection = s->get("rstdir").compare("true") == 0 ? true : false;

        // weight
        Weight *wh = new Weight( object->get("weight").toInt(), keeper );
        ui->weightLabel->setText(wh->get("title"));

        display->setStyleLabel( QString("%1 %2 KG").arg(s->get("name"), wh->get("title") ) );

        ui->numLabel->setText( object->get("num") );
        display->setNumberLabel( object->get("num") );

        delete wh;
        delete s;
        delete g;

        ui->roundOneButton->setEnabled(true);
        ui->endofmatchButton->setEnabled(true);

        ui->monitorButton->toggle();
        display->showDisplay();

        if ( isGR_18052013 ) {
            ui->plusRedPoint3->hide();
            ui->redPoint3->hide();
            ui->minusRedPoint3->hide();
            ui->redThirdWin->hide();

            ui->blueThirdWin->hide();
            ui->plusBluePoint3->hide();
            ui->bluePoint3->hide();
            ui->minusBluePoint3->hide();

            ui->roundThreeButton->hide();
            ui->round3ovetime->hide();


        } else {
            ui->plusRedPoint3->show();
            ui->redPoint3->show();
            ui->minusRedPoint3->show();
            ui->redThirdWin->show();

            ui->blueThirdWin->show();
            ui->plusBluePoint3->show();
            ui->bluePoint3->show();
            ui->minusBluePoint3->show();

            ui->roundThreeButton->show();
            ui->round3ovetime->show();
        }

        display->toggleGR18052013(isGR_18052013);

    }

    void FightControl::initRoundButtons()
    {
        QSignalMapper *map = 0;

        map = new QSignalMapper(this);

        connect( ui->roundOneButton,   SIGNAL(released()), map, SLOT(map()));
        connect( ui->roundTwoButton,   SIGNAL(released()), map, SLOT(map()));
        connect( ui->roundThreeButton, SIGNAL(released()), map, SLOT(map()));
        connect( ui->breaktimeButton,  SIGNAL(released()), map, SLOT(map()));

        map->setMapping(ui->roundOneButton, 1);
        map->setMapping(ui->roundTwoButton, 2);
        map->setMapping(ui->roundThreeButton, 3);
        map->setMapping(ui->breaktimeButton, 4);

        connect(map, SIGNAL(mapped(int)), this, SLOT(roundChanged(int)));

        // ------

        map = new QSignalMapper(this);

        connect( ui->round1ovetime,   SIGNAL(released()), map, SLOT(map()));
        connect( ui->round2ovetime,   SIGNAL(released()), map, SLOT(map()));
        connect( ui->round3ovetime, SIGNAL(released()), map, SLOT(map()));

        map->setMapping(ui->round1ovetime, 1);
        map->setMapping(ui->round2ovetime, 2);
        map->setMapping(ui->round3ovetime, 3);

        connect(map, SIGNAL(mapped(int)), this, SLOT(roundOvertime_pressed(int)));
    }

    void FightControl::initButtons()
    {
        initRoundButtons();

        connect (ui->selectMatch, SIGNAL(clicked()), this, SLOT(selectMatch_clicked()));

        connect( ui->startPauseButton, SIGNAL(toggled(bool)), this, SLOT(startPauseBatton_toggled(bool)));
        connect( ui->stopButton,       SIGNAL(clicked()), this, SLOT(stopButton_clicked()));

        connect( ui->plusTimeButton,   SIGNAL(clicked()), this, SLOT(increaseTimer()));
        connect( ui->minusTimeButton,  SIGNAL(clicked()), this, SLOT(decreaseTimer()));

        connect( ui->splashButton, SIGNAL(pressed()), this, SLOT(showSplash()));
        connect( ui->monitorButton, SIGNAL(pressed()), this, SLOT(showDisplay()));

        connect( ui->endofmatchButton,  SIGNAL(pressed()), this, SLOT(endofmatchButton_pressed()));

        // --red
        connect( ui->plusRedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint_clicked()));
        connect( ui->minusRedPoint, SIGNAL(clicked()), this, SLOT(redMinusPoint_clicked()));

        connect( ui->plusRedBan, SIGNAL(clicked()), this, SLOT(redPlusBan_clicked()));
        connect( ui->minusRedBan, SIGNAL(clicked()), this, SLOT(redMinusBan_clicked()));

        connect( ui->plus1RedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint1_clicked()));
        connect( ui->plus2RedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint2_clicked()));
        connect( ui->plus3RedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint3_clicked()));
        connect( ui->plus5RedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint5_clicked()));

        // -- blue
        connect( ui->plusBluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint_clicked()));
        connect( ui->minusBluePoint, SIGNAL(clicked()), this, SLOT(blueMinusPoint_clicked()));

        connect( ui->plusBlueBan, SIGNAL(clicked()), this, SLOT(bluePlusBan_clicked()));
        connect( ui->minusBlueBan, SIGNAL(clicked()), this, SLOT(blueMinusBan_clicked()));

        connect( ui->plus1BluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint1_clicked()));
        connect( ui->plus2BluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint2_clicked()));
        connect( ui->plus3BluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint3_clicked()));
        connect( ui->plus5BluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint5_clicked()));


    }

    void FightControl::initSound()
    {
        QSettings settings;
        player = Phonon::createPlayer(Phonon::MusicCategory);

        sound.append( new Phonon::MediaSource(settings.value("sound/001",
                                                             QApplication::applicationDirPath() + "/share/sound/001.wav").toString()));
        sound.append( new Phonon::MediaSource(settings.value("sound/002",
                                                             QApplication::applicationDirPath() + "/share/sound/002.wav").toString()));
        sound.append( new Phonon::MediaSource(settings.value("sound/003",
                                                             QApplication::applicationDirPath() + "/share/sound/003.wav").toString()));
        sound.append( new Phonon::MediaSource(settings.value("sound/004",
                                                             QApplication::applicationDirPath() + "/share/sound/004.wav").toString()));


        QSignalMapper *map1 = new QSignalMapper(this);
        connect( ui->stopButton,   SIGNAL(clicked()), map1, SLOT(map()));
        connect( ui->sndButton1,   SIGNAL(clicked()), map1, SLOT(map()));
        connect( ui->sndButton2,   SIGNAL(clicked()), map1, SLOT(map()));

        map1->setMapping(ui->stopButton, 0);
        map1->setMapping(ui->sndButton1, 1);
        map1->setMapping(ui->sndButton2, 2);

        connect(map1, SIGNAL(mapped(int)), this, SLOT(playSound(int)));
    }

    void FightControl::initPointManage()
    {
        redPoints.append( ui->redPoint1 );
        redPoints.append( ui->redPoint2 );
        redPoints.append( ui->redPoint3 );

        bluePoints.append( ui->bluePoint1 );
        bluePoints.append( ui->bluePoint2 );
        bluePoints.append( ui->bluePoint3 );

        QSignalMapper *map;

        // ---------
        map = new QSignalMapper(this);
        connect( ui->plusRedPoint1,   SIGNAL(clicked()), map, SLOT(map()));
        connect( ui->plusRedPoint2,   SIGNAL(clicked()), map, SLOT(map()));
        connect( ui->plusRedPoint3,   SIGNAL(clicked()), map, SLOT(map()));

        map->setMapping(ui->plusRedPoint1, 0);
        map->setMapping(ui->plusRedPoint2, 1);
        map->setMapping(ui->plusRedPoint3, 2);

        connect(map, SIGNAL(mapped(int)), this, SLOT(plusRedPoint(int)));

        // ----------
        map = new QSignalMapper(this);
        connect( ui->minusRedPoint1,   SIGNAL(clicked()), map, SLOT(map()));
        connect( ui->minusRedPoint2,   SIGNAL(clicked()), map, SLOT(map()));
        connect( ui->minusRedPoint3,   SIGNAL(clicked()), map, SLOT(map()));

        map->setMapping(ui->minusRedPoint1, 0);
        map->setMapping(ui->minusRedPoint2, 1);
        map->setMapping(ui->minusRedPoint3, 2);

        connect(map, SIGNAL(mapped(int)), this, SLOT(minusRedPoint(int)));

        // ---------
        map = new QSignalMapper(this);
        connect( ui->plusBluePoint1,   SIGNAL(clicked()), map, SLOT(map()));
        connect( ui->plusBluePoint2,   SIGNAL(clicked()), map, SLOT(map()));
        connect( ui->plusBluePoint3,   SIGNAL(clicked()), map, SLOT(map()));

        map->setMapping(ui->plusBluePoint1, 0);
        map->setMapping(ui->plusBluePoint2, 1);
        map->setMapping(ui->plusBluePoint3, 2);

        connect(map, SIGNAL(mapped(int)), this, SLOT(plusBluePoint(int)));

        // ----------
        map = new QSignalMapper(this);
        connect( ui->minusBluePoint1,   SIGNAL(clicked()), map, SLOT(map()));
        connect( ui->minusBluePoint2,   SIGNAL(clicked()), map, SLOT(map()));
        connect( ui->minusBluePoint3,   SIGNAL(clicked()), map, SLOT(map()));

        map->setMapping(ui->minusBluePoint1, 0);
        map->setMapping(ui->minusBluePoint2, 1);
        map->setMapping(ui->minusBluePoint3, 2);

        connect(map, SIGNAL(mapped(int)), this, SLOT(minusBluePoint(int)));


        redWin.append(ui->redFirstWin);
        redWin.append(ui->redSecondWin);
        redWin.append(ui->redThirdWin);

        map = new QSignalMapper(this);
        connect(ui->redFirstWin, SIGNAL(clicked()), map, SLOT(map()));
        connect(ui->redSecondWin, SIGNAL(clicked()), map, SLOT(map()));
        connect(ui->redThirdWin, SIGNAL(clicked()), map, SLOT(map()));

        map->setMapping(ui->redFirstWin, 0);
        map->setMapping(ui->redSecondWin, 1);
        map->setMapping(ui->redThirdWin, 2);

        connect(map, SIGNAL(mapped(int)), this, SLOT(setRedRoundWinner(int)));

        blueWin.append(ui->blueFirstWin);
        blueWin.append(ui->blueSecondWin);
        blueWin.append(ui->blueThirdWin);

        map = new QSignalMapper(this);
        connect(ui->blueFirstWin, SIGNAL(clicked()), map, SLOT(map()));
        connect(ui->blueSecondWin, SIGNAL(clicked()), map, SLOT(map()));
        connect(ui->blueThirdWin, SIGNAL(clicked()), map, SLOT(map()));

        map->setMapping(ui->blueFirstWin, 0);
        map->setMapping(ui->blueSecondWin, 1);
        map->setMapping(ui->blueThirdWin, 2);

        connect(map, SIGNAL(mapped(int)), this, SLOT(setBlueRoundWinner(int)));
    }

    void FightControl::setRedRoundWinner(int index)
    {
        QCheckBox *redBox = redWin.at(index);
        QCheckBox *blueBox = blueWin.at(index);
        if ( redBox && redBox->isChecked() ) {
            blueBox->setChecked(false);

            // set to display
            display->setRoundWinner(index+1, true);
        }
    }

    void FightControl::setBlueRoundWinner(int index)
    {
        QCheckBox *redBox = redWin.at(index);
        QCheckBox *blueBox = blueWin.at(index);
        if ( blueBox && blueBox->isChecked() ) {
            redBox->setChecked(false);

            // set to display
            display->setRoundWinner(index+1, false);
        }
    }

    void FightControl::roundOvertime_pressed(int index)
    {
        startMilliseconds = overtime * 1000;
        stopMilliseconds = 0;
        timerOrder = overtimedirection ? Ascendant : Descendant;

        setTimer();

        if ( index == 1 ) {
            ui->round2ovetime->setChecked(false);
            ui->round3ovetime->setChecked(false);
        } else if ( index == 2 ) {
            ui->round1ovetime->setChecked(false);
            ui->round3ovetime->setChecked(false);
        } else if ( index == 3 ) {
            ui->round1ovetime->setChecked(false);
            ui->round2ovetime->setChecked(false);
        }

        ui->breaktimeButton->setEnabled(false);
        ui->startPauseButton->setEnabled(true);
    }

    void FightControl::plusRedPoint(int index)
    {
        int value = redPoints.at(index)->text().toInt();
        redPoints.at(index)->setText( QString::number( ++value ) );
//        display->setRedRoundResult(index+1, QString::number(value) );

        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("score")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", QString::number(1));
        a->set("wrestler", red->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::minusRedPoint(int index)
    {
        int value = redPoints.at(index)->text().toInt();
        if ( --value < 0 ) return;
        redPoints.at(index)->setText( QString::number( value ) );
//        display->setRedRoundResult(index+1, QString::number(value) );

        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("score")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", QString::number(-1));
        a->set("wrestler", red->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::plusBluePoint(int index)
    {
        int value = bluePoints.at(index)->text().toInt();
        bluePoints.at(index)->setText( QString::number( ++value ) );
//        display->setBlueRoundResult(index+1, QString::number(value) );

        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("score")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", QString::number(1));
        a->set("wrestler", blue->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::minusBluePoint(int index)
    {
        int value = bluePoints.at(index)->text().toInt();
        if ( --value < 0 ) return;
        bluePoints.at(index)->setText( QString::number( value ) );
//        display->setBlueRoundResult(index+1, QString::number(value) );

        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("score")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", QString::number(-1));
        a->set("wrestler", blue->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::showDisplay()
    {
        if ( !display->isVisible() )
            display->show();

        display->showDisplay();
    }

    void FightControl::showSplash()
    {
        if ( !display->isVisible() )
            display->show();

        display->showSplash();
    }

    void FightControl::endofmatchButton_pressed()
    {
        stopButton_clicked();

        WinnerDialog dialog(object, keeper, this);
        if ( dialog.exec() == QDialog::Accepted )
        {
            delete object;
            object = 0;

            FightLog *log  = 0;
            for( int i = 0; i < pendingActions.size(); i++ )
            {
                log = qobject_cast<FightLog*>(pendingActions.at(i));
                try
                {
                    log->store();
                } catch ( SqlError &/*err*/ ) {
//                    QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
                }
            }
        }

        qDeleteAll(pendingActions);
        pendingActions.clear();


        ui->selectMatch->setEnabled(true);

        initButtonStates();
    }

    void FightControl::increaseTimer()
    {
        if ( timerOrder == Descendant ) {
            startMilliseconds += 30 * 1000;
        } else {
            stopMilliseconds += 30 * 1000;
        }
        setTimer();
    }

    void FightControl::decreaseTimer()
    {
        int *timeToUse = &(timerOrder == Descendant ? startMilliseconds : stopMilliseconds);
        if ( 0 < ((*timeToUse)-(30*1000)) )
            (*timeToUse) -= 30 * 1000;
        else
            (*timeToUse) = 0;

        setTimer();
    }

    void FightControl::setTimer()
    {
        int timeUse = timerOrder == Ascendant ? (startMilliseconds) : qAbs(stopMilliseconds-startMilliseconds);

        int Minutes = (timeUse % (1000*60*60)) / (1000*60);
        int Seconds = ((timeUse % (1000*60*60)) % (1000*60)) / 1000;

        QString time2 = QString("%1:%2").arg(Minutes).arg(Seconds, 2, 10, QChar('0'));
        ui->timerLabel->setText( time2 );
        display->setTimer( time2 );
    }

    void  FightControl::timeOut()
    {
        int step = timerOrder == Ascendant ? 1000 : -1000;
        int time = timerOrder == Descendant ? (startMilliseconds) : (stopMilliseconds-startMilliseconds);

        if (time + step >= 0 ) {
            startMilliseconds += step;
            setTimer();
        }

        time = timerOrder == Descendant ? (startMilliseconds) : (stopMilliseconds-startMilliseconds);

        if ( time <= 0 )
        {
            stopButton_clicked();
            playSound(0);
        }

    }

    void FightControl::startPauseBatton_toggled(bool flag)
    {
        ui->stopButton->setEnabled(true);
        if ( flag )
        {
            ui->stopButton->setChecked(false);
            ui->startPauseButton->setText( tr("Pause") );
            ui->startPauseButton->setIcon( QIcon(":/icons/clock_pause.png"));
            countdown->start();

            FightLog *a = new FightLog(keeper);
            a->set("fight", object->get("id"));
            a->set("action", QString::number(actions.value("start")));
            a->set("fround", QString::number(currentRound()));
            a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            pendingActions.append(a);

            if ( !ui->breaktimeButton->isChecked() )
            {
                ui->plusRedBan->setEnabled(true);
                ui->minusRedBan->setEnabled(true);

                ui->plusBlueBan->setEnabled(true);
                ui->minusBlueBan->setEnabled(true);

                ui->plus1RedPoint->setEnabled(true);
                ui->plus2RedPoint->setEnabled(true);
                ui->plus3RedPoint->setEnabled(true);
                ui->plus5RedPoint->setEnabled(true);

                ui->plus1BluePoint->setEnabled(true);
                ui->plus2BluePoint->setEnabled(true);
                ui->plus3BluePoint->setEnabled(true);
                ui->plus5BluePoint->setEnabled(true);

                ui->minusRedPoint->setEnabled(true);
                ui->plusRedPoint->setEnabled(true);

                ui->minusBluePoint->setEnabled(true);
                ui->plusBluePoint->setEnabled(true);

                ui->minusTimeButton->setEnabled(true);
                ui->plusTimeButton->setEnabled(true);
            }
        } else {
            countdown->stop();
            ui->startPauseButton->setText( tr("Start") );
            ui->startPauseButton->setIcon( QIcon(":/icons/clock_play.png"));

            FightLog *a = new FightLog(keeper);
            a->set("fight", object->get("id"));
            a->set("action", QString::number(actions.value("pause")));
            a->set("fround", QString::number(currentRound()));
            a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            pendingActions.append(a);
        }

    }

    void FightControl::stopButton_clicked()
    {
        if ( ui->startPauseButton->isChecked() )
            ui->startPauseButton->toggle();

        ui->stopButton->setEnabled(false);
        ui->startPauseButton->setEnabled(false);

        countdown->stop();

        ui->minusRedBan->setEnabled(false);
        ui->plusRedBan->setEnabled(false);

        ui->minusBlueBan->setEnabled(false);
        ui->plusBlueBan->setEnabled(false);

        ui->plus1RedPoint->setEnabled(false);
        ui->plus2RedPoint->setEnabled(false);
        ui->plus3RedPoint->setEnabled(false);
        ui->plus5RedPoint->setEnabled(false);

        ui->plus1BluePoint->setEnabled(false);
        ui->plus2BluePoint->setEnabled(false);
        ui->plus3BluePoint->setEnabled(false);
        ui->plus5BluePoint->setEnabled(false);

        ui->minusRedPoint->setEnabled(false);
        ui->plusRedPoint->setEnabled(false);

        ui->minusBluePoint->setEnabled(false);
        ui->plusBluePoint->setEnabled(false);

        ui->minusTimeButton->setEnabled(false);
        ui->plusTimeButton->setEnabled(false);


        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("stop")));
        a->set("fround", QString::number(currentRound()));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);

        QString blue = ui->bluePoint->text();
        QString red = ui->redPoint->text();
//        bool winner = red.toInt() > blue.toInt();

        ui->breaktimeButton->setChecked(false);

        if ( ui->roundOneButton->isChecked() ) {
            ui->bluePoint1->setText( blue );
            ui->redPoint1->setText( red );

            ui->round1ovetime->setEnabled(true);
            ui->breaktimeButton->setEnabled(true);

            ui->plusRedPoint1->setEnabled(true);
            ui->minusRedPoint1->setEnabled(true);

            ui->plusBluePoint1->setEnabled(true);
            ui->minusBluePoint1->setEnabled(true);

            ui->redFirstWin->setEnabled(true);
            ui->blueFirstWin->setEnabled(true);
        }
        else if ( ui->roundTwoButton->isChecked() ) {
            ui->bluePoint2->setText( blue );
            ui->redPoint2->setText( red );

            ui->round2ovetime->setEnabled(true);
            ui->breaktimeButton->setEnabled(true);

            ui->plusRedPoint2->setEnabled(true);
            ui->minusRedPoint2->setEnabled(true);

            ui->plusBluePoint2->setEnabled(true);
            ui->minusBluePoint2->setEnabled(true);

            ui->redSecondWin->setEnabled(true);
            ui->blueSecondWin->setEnabled(true);
        }
        else if ( ui->roundThreeButton->isChecked() )
        {
            ui->bluePoint3->setText( blue );
            ui->redPoint3->setText( red );

            ui->round3ovetime->setEnabled(true);
            ui->breaktimeButton->setEnabled(true);

            ui->plusRedPoint3->setEnabled(true);
            ui->minusRedPoint3->setEnabled(true);

            ui->plusBluePoint3->setEnabled(true);
            ui->minusBluePoint3->setEnabled(true);

            ui->redThirdWin->setEnabled(true);
            ui->blueThirdWin->setEnabled(true);
        }

        if ( ui->roundOneButton->isChecked() || ui->round1ovetime->isChecked() ) {
            int rp = ui->redPoint1->text().toInt();
            int bp = ui->bluePoint1->text().toInt();
            if ( rp > bp ) {
                ui->redFirstWin->setChecked(true);
                setRedRoundWinner(0);
            } else if ( bp > rp ) {
                ui->blueFirstWin->setChecked(true);
                setBlueRoundWinner(0);
            }
        } else if ( ui->roundTwoButton->isChecked() || ui->round2ovetime->isChecked() ) {
            int rp = ui->redPoint2->text().toInt();
            int bp = ui->bluePoint2->text().toInt();
            if ( rp > bp ) {
                ui->redSecondWin->setChecked(true);
                setRedRoundWinner(1);
            } else if ( bp > rp ) {
                ui->blueSecondWin->setChecked(true);
                setBlueRoundWinner(1);
            }
        } else if ( ui->roundThreeButton->isChecked() || ui->round3ovetime->isChecked() ) {
            int rp = ui->redPoint3->text().toInt();
            int bp = ui->bluePoint3->text().toInt();
            if ( rp > bp ) {
                ui->redThirdWin->setChecked(true);
                setRedRoundWinner(2);
            } else if ( bp > rp ) {
                ui->blueThirdWin->setChecked(true);
                setBlueRoundWinner(2);
            }
        }
    }

    void FightControl::playSound(int index)
    {
        if ( index == 0 ) {
            if ( ui->stopSound->isChecked() ) {
                player->setCurrentSource( *(sound.at(index)) );
                player->play();
            }
        } else {
            player->setCurrentSource( *(sound.at(index)) );
            player->play();
        }
    }

    void FightControl::roundChanged(int i )
    {
        QString text;

        timerOrder = roundtimedirection ? Ascendant : Descendant;

        if ( i == 1 )
        {
            text = rounds["first"].at(1);
            ui->roundTwoButton->setChecked(false);
            ui->round2ovetime->setChecked(false);
            ui->roundThreeButton->setChecked(false);
            ui->round3ovetime->setChecked(false);
            ui->breaktimeButton->setChecked(false);
            ui->breaktimeButton->setEnabled(false);
            ui->endofmatchButton->setChecked(false);
        }
        else if ( i == 2 )
        {
            text = rounds["second"].at(1);
            ui->roundOneButton->setChecked(false);
            ui->round1ovetime->setChecked(false);
            ui->roundThreeButton->setChecked(false);
            ui->round3ovetime->setChecked(false);
            ui->breaktimeButton->setChecked(false);
            ui->breaktimeButton->setEnabled(false);
        }
        else if ( i == 3 )
        {
            text = rounds["third"].at(1);
            ui->roundTwoButton->setChecked(false);
            ui->round2ovetime->setChecked(false);
            ui->roundOneButton->setChecked(false);
            ui->round1ovetime->setChecked(false);
            ui->breaktimeButton->setChecked(false);
            ui->breaktimeButton->setEnabled(false);
        }
        else if ( i == 4 )
        {
            ui->roundOneButton->setChecked(false);
            ui->round1ovetime->setChecked(false);
            ui->roundTwoButton->setChecked(false);
            ui->round2ovetime->setChecked(false);
            ui->roundThreeButton->setChecked(false);
            ui->round3ovetime->setChecked(false);
            text = rounds["breaktime"].at(1);

            timerOrder = resttimedirection ? Ascendant : Descendant;

            if ( lastRound == 1 ) {
                ui->roundOneButton->setEnabled(false);
                ui->round1ovetime->setEnabled(false);
                ui->roundTwoButton->setEnabled(true);
            } else if ( lastRound == 2 ) {
                ui->roundTwoButton->setEnabled(false);
                ui->round2ovetime->setEnabled(false);
                ui->roundThreeButton->setEnabled(true);
            } else if ( lastRound == 3 ) {
                ui->roundThreeButton->setEnabled(false);
                ui->round3ovetime->setEnabled(false);
            }
        }

        if ( i < 4 )
        {
            lastRound = i;

            if ( timerOrder == Ascendant )
            {
                startMilliseconds = 0;
                stopMilliseconds = roundtime * 1000;
            } else {
                startMilliseconds = roundtime * 1000;
                stopMilliseconds = 0;
            }
        } else {
            timerOrder = Descendant;
            startMilliseconds = overtime * 1000;
            stopMilliseconds = 0;
        }

        ui->startPauseButton->setEnabled(false);


        if ( (ui->roundOneButton->isChecked() || ui->round1ovetime->isChecked())
             || (ui->roundTwoButton->isChecked() || ui->round2ovetime->isChecked())
             || (ui->roundThreeButton->isChecked() || ui->round3ovetime->isChecked())
             || ui->breaktimeButton->isChecked() )
        {
            ui->startPauseButton->setEnabled(true);
        }

        setTimer();

        display->setRound(text);

        if ( !isGR_18052013 ) {
            ui->bluePoint->setText(QString("0"));
            ui->redPoint->setText(QString("0"));

            display->setBluePoint(QString("0"));
            display->setRedPoint(QString("0"));
        }
    }

    void FightControl::openMonitor_clicked()
    {
        display->setVisible( !display->isVisible() );
    }

    void FightControl::redPlusBan_clicked()
    {
        if ( !ui->redBan1->isChecked() ) {
            ui->redBan1->setChecked(true);
            display->setRedBan(1);
        } else if ( !ui->redBan2->isChecked() ) {
            ui->redBan2->setChecked(true);
            display->setRedBan(2);
        } else if ( !ui->redBan3->isChecked()) {
            ui->redBan3->setChecked(true);
            display->setRedBan(3);
        }
        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("pen")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", "1");
        a->set("wrestler", red->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::redMinusBan_clicked()
    {
        if ( ui->redBan3->isChecked() ) {
            ui->redBan3->setChecked(false);
            display->clearRedBan(3);
        } else if ( ui->redBan2->isChecked() ) {
            ui->redBan2->setChecked(false);
            display->clearRedBan(2);
        } else if ( ui->redBan1->isChecked()) {
            ui->redBan1->setChecked(false);
            display->clearRedBan(1);
        }
        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("pen")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", "-1");
        a->set("wrestler", red->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::bluePlusBan_clicked()
    {
        if ( !ui->blueBan1->isChecked() ) {
            ui->blueBan1->setChecked(true);
            display->setBlueBan(1);
        } else if ( !ui->blueBan2->isChecked() ) {
            ui->blueBan2->setChecked(true);
            display->setBlueBan(2);
        } else if ( !ui->blueBan3->isChecked()) {
            ui->blueBan3->setChecked(true);
            display->setBlueBan(3);
        }
        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("pen")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", "1");
        a->set("wrestler", blue->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::blueMinusBan_clicked()
    {
        if ( ui->blueBan3->isChecked() ) {
            ui->blueBan3->setChecked(false);
            display->clearBlueBan(3);
        } else if ( ui->blueBan2->isChecked() ) {
            ui->blueBan2->setChecked(false);
            display->clearBlueBan(2);
        } else if ( ui->blueBan1->isChecked()) {
            ui->blueBan1->setChecked(false);
            display->clearBlueBan(1);
        }
        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("pen")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", "-1");
        a->set("wrestler", blue->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::setRedPoint(int p)
    {
        int point = ui->redPoint->text().toInt();
        if ( 0 <= (point + p) )
            point += p;
        ui->redPoint->setText( QString::number(point) );
        display->setRedPoint( QString::number(point) );

        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("score")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", QString::number(p));
        a->set("wrestler", red->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }



    void FightControl::redPlusPoint_clicked() { setRedPoint(1); }
    void FightControl::redMinusPoint_clicked() { setRedPoint(-1); }

    void FightControl::redPlusPoint1_clicked() { setRedPoint(1); }
    void FightControl::redPlusPoint2_clicked() { setRedPoint(2); }
    void FightControl::redPlusPoint3_clicked() { setRedPoint(3); }
    void FightControl::redPlusPoint5_clicked() { setRedPoint(5); }

    void FightControl::setBluePoint(int p)
    {
        int point = ui->bluePoint->text().toInt();
        if ( 0 <= (point + p) )
            point += p;
        ui->bluePoint->setText( QString::number(point) );
        display->setBluePoint( QString::number(point) );

        FightLog *a = new FightLog(keeper);
        a->set("fight", object->get("id"));
        a->set("action", QString::number(actions.value("score")));
        a->set("fround", QString::number(currentRound()));
        a->set("score", QString::number(p));
        a->set("wrestler", blue->get("id"));
        a->set("etime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        pendingActions.append(a);
    }

    void FightControl::bluePlusPoint_clicked() { setBluePoint(1); }
    void FightControl::blueMinusPoint_clicked() { setBluePoint(-1); }


    void FightControl::bluePlusPoint1_clicked() { setBluePoint(1); }
    void FightControl::bluePlusPoint2_clicked() { setBluePoint(2); }
    void FightControl::bluePlusPoint3_clicked() { setBluePoint(3); }
    void FightControl::bluePlusPoint5_clicked() { setBluePoint(5); }



    void FightControl::changeSettings()
    {
        while( sound.size() )
            delete sound.takeFirst();

        QSettings settings;
        sound.append( new Phonon::MediaSource(settings.value("sound/001",
                                                             QApplication::applicationDirPath() + "/sound/001.wav").toString()));
        sound.append( new Phonon::MediaSource(settings.value("sound/002",
                                                             QApplication::applicationDirPath() + "/sound/002.wav").toString()));
        sound.append( new Phonon::MediaSource(settings.value("sound/003",
                                                             QApplication::applicationDirPath() + "/sound/003.wav").toString()));
        sound.append( new Phonon::MediaSource(settings.value("sound/004",
                                                             QApplication::applicationDirPath() + "/sound/004.wav").toString()));

        emit settingsChanged();
    }

    int FightControl::currentRound()
    {
        QString round;
        if ( ui->roundOneButton->isChecked() )
            round = rounds.value("first").at(0);
        else if ( ui->roundTwoButton->isChecked() )
            round = rounds.value("second").at(0);
        else if ( ui->roundThreeButton->isChecked() )
            round = rounds.value("third").at(0);
        else if ( ui->round1ovetime->isChecked() )
            round = rounds.value("first.overtime").at(0);
        else if ( ui->round2ovetime->isChecked() )
            round = rounds.value("second.overtime").at(0);
        else if ( ui->round3ovetime->isChecked() )
            round = rounds.value("third.overtime").at(0);
        else if ( ui->breaktimeButton->isChecked() )
            round = rounds.value("breaktime").at(0);

        return round.toInt();
    }
}
