#include "fightcontrol.h"
#include "ui_fightcontrol.h"

#include "fightdisplay.h"

#include <QtGui>
#include <QSvgWidget>
#include <QFileDialog>
#include <QTimer>
#include <QSettings>
#include <QLineEdit>
#include <phonon>

FightControl::FightControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FightControl)
{
    ui->setupUi(this);

    display = new FightDisplay;
    display->hide();

    connect(this, SIGNAL(settingsChanged()), display, SLOT(changeSettings()));

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

    ui->startPauseButton->setEnabled(false);

    startMilliseconds = 0;
    stopMilliseconds = 0;

    setTimer();

    initButtonStates();

    timerOrder = Descendant;
    lastRound = 0;
}

void FightControl::keyPressEvent ( QKeyEvent * event )
{
    if ( event->key() == Qt::Key_Escape) {
        emit ui->sndButton1->click();
    }
}

void FightControl::initButtonStates()
{
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
}

FightControl::~FightControl()
{
    delete display;
    delete ui;
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

    connect( ui->styleCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(styleCombo_changed(QString)));
    connect( ui->weightEdit, SIGNAL(textChanged(QString)), this, SLOT(weightEdit_changed(QString)));
    connect( ui->numEdit,    SIGNAL(textChanged(QString)), this, SLOT(numEdit_changed(QString)));

    connect( ui->startPauseButton, SIGNAL(toggled(bool)), this, SLOT(startPauseBatton_toggled(bool)));
    connect( ui->stopButton,       SIGNAL(clicked()), this, SLOT(stopButton_clicked()));

    connect( ui->plusTimeButton,   SIGNAL(clicked()), this, SLOT(increaseTimer()));
    connect( ui->minusTimeButton,  SIGNAL(clicked()), this, SLOT(decreaseTimer()));

    connect( ui->splashButton, SIGNAL(pressed()), this, SLOT(showSplash()));
    connect( ui->monitorButton, SIGNAL(pressed()), this, SLOT(showDisplay()));

    connect( ui->endofmatchButton,  SIGNAL(pressed()), this, SLOT(endofmatchButton_pressed()));

    // --red
    connect( ui->redFlagButton, SIGNAL(clicked()), this, SLOT(redFlagButton_clicked()) );
    connect( ui->clearRedFlag, SIGNAL(clicked()), this, SLOT(clearRedFlag_clicked()) );

    connect( ui->redCountryEdit, SIGNAL(textChanged(QString)), this, SLOT(redCountry_changed()));
    connect( ui->clearRedCountry, SIGNAL(clicked()), ui->redCountryEdit, SLOT(clear()));

    connect( ui->redWrestlerEdit, SIGNAL(textChanged(QString)), this, SLOT(redWrestler_changed()));
    connect( ui->clearRedWrestler, SIGNAL(clicked()), ui->redWrestlerEdit, SLOT(clear()));

    connect( ui->plusRedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint_clicked()));
    connect( ui->minusRedPoint, SIGNAL(clicked()), this, SLOT(redMinusPoint_clicked()));

    connect( ui->plusRedBan, SIGNAL(clicked()), this, SLOT(redPlusBan_clicked()));
    connect( ui->minusRedBan, SIGNAL(clicked()), this, SLOT(redMinusBan_clicked()));

    connect( ui->plus1RedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint1_clicked()));
    connect( ui->plus2RedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint2_clicked()));
    connect( ui->plus3RedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint3_clicked()));
    connect( ui->plus5RedPoint, SIGNAL(clicked()), this, SLOT(redPlusPoint5_clicked()));

    // -- blue

    connect( ui->blueFlagButton, SIGNAL(clicked()), this, SLOT(blueFlagButton_clicked()) );
    connect( ui->clearBlueFlag, SIGNAL(clicked()), this, SLOT(clearBlueFlag_clicked()) );

    connect( ui->blueCountryEdit, SIGNAL(textChanged(QString)), this, SLOT(blueCountry_changed()));
    connect( ui->clearBlueCountry, SIGNAL(clicked()), ui->blueCountryEdit, SLOT(clear()));

    connect( ui->blueWrestlerEdit, SIGNAL(textChanged(QString)), this, SLOT(blueWrestler_changed()));
    connect( ui->clearBlueWrestler, SIGNAL(clicked()), ui->blueWrestlerEdit, SLOT(clear()));

    connect( ui->plusBluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint_clicked()));
    connect( ui->minusBluePoint, SIGNAL(clicked()), this, SLOT(blueMinusPoint_clicked()));

    connect( ui->plusBlueBan, SIGNAL(clicked()), this, SLOT(bluePlusBan_clicked()));
    connect( ui->minusBlueBan, SIGNAL(clicked()), this, SLOT(blueMinusBan_clicked()));

    connect( ui->plus1BluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint1_clicked()));
    connect( ui->plus2BluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint2_clicked()));
    connect( ui->plus3BluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint3_clicked()));
    connect( ui->plus5BluePoint, SIGNAL(clicked()), this, SLOT(bluePlusPoint5_clicked()));

    initSound();
    initPointManage();
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
}

void FightControl::roundOvertime_pressed(int index)
{
    startMilliseconds = 30 * 1000;
    stopMilliseconds = 0;
    timerOrder = Descendant;

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
}

void FightControl::plusRedPoint(int index)
{
    int value = redPoints.at(index)->text().toInt();
    redPoints.at(index)->setText( QString::number( ++value ) );
    display->setRedRoundResult(index+1, QString::number(value) );
}

void FightControl::minusRedPoint(int index)
{
    int value = redPoints.at(index)->text().toInt();
    if ( --value < 0 ) return;
    redPoints.at(index)->setText( QString::number( value ) );
    display->setRedRoundResult(index+1, QString::number(value) );
}

void FightControl::plusBluePoint(int index)
{
    int value = bluePoints.at(index)->text().toInt();
    bluePoints.at(index)->setText( QString::number( ++value ) );
    display->setBlueRoundResult(index+1, QString::number(value) );
}

void FightControl::minusBluePoint(int index)
{
    int value = bluePoints.at(index)->text().toInt();
    if ( --value < 0 ) return;
    bluePoints.at(index)->setText( QString::number( value ) );
    display->setBlueRoundResult(index+1, QString::number(value) );
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
    ui->startPauseButton->setEnabled(false);
    startMilliseconds = 0;
    stopMilliseconds = 0;
    setTimer();

    redFlagWidget->load(QByteArray());
    display->setRedFlag(QString(""));

    ui->redCountryEdit->clear();
    ui->redWrestlerEdit->clear();
    ui->redPoint1->clear();
    ui->redPoint2->clear();
    ui->redPoint3->clear();
    ui->redPoint->setText(QString("0"));

    blueFlagWidget->load(QByteArray());
    display->setBlueFlag(QString(""));

    ui->blueCountryEdit->clear();
    ui->blueWrestlerEdit->clear();
    ui->bluePoint1->clear();
    ui->bluePoint2->clear();
    ui->bluePoint3->clear();
    ui->bluePoint->setText(QString("0"));

    ui->styleCombo->setCurrentIndex(0);
    ui->weightEdit->clear();
    ui->numEdit->clear();

    QString empty = QString("");
    display->setRoundResult(1, empty, empty);
    display->setRoundResult(2, empty, empty);
    display->setRoundResult(3, empty, empty);

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

    display->clearRedBan(1);
    display->clearRedBan(2);
    display->clearRedBan(3);

    display->clearBlueBan(1);
    display->clearBlueBan(2);
    display->clearBlueBan(3);

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

//    if ( ui->styleCombo->currentIndex() == 2 &&
//         time == 30*1000 )
//    {
//        playSound(1);
//    }

    if ( time <= 0 )
    {
        stopButton_clicked();
        playSound(0);
    }

}

void FightControl::startPauseBatton_toggled(bool flag)
{
    if ( flag )
    {
        ui->stopButton->setChecked(false);
        ui->startPauseButton->setText( tr("Pause") );
        ui->startPauseButton->setIcon( QIcon(":/icons/clock_pause.png"));
        countdown->start();
    } else {
        countdown->stop();
        ui->startPauseButton->setText( tr("Start") );
        ui->startPauseButton->setIcon( QIcon(":/icons/clock_play.png"));
    }

}

void FightControl::stopButton_clicked()
{
    if ( ui->startPauseButton->isChecked() )
        ui->startPauseButton->toggle();

    // startMilliseconds = 0;
    // stopMilliseconds = 0;

    // setTimer();

    countdown->stop();

    QString blue = ui->bluePoint->text();
    QString red = ui->redPoint->text();

    ui->breaktimeButton->setChecked(false);
    // ui->breaktimeButton->setEnabled(false);

    if ( ui->roundOneButton->isChecked() ) {
        ui->bluePoint1->setText( blue );
        ui->redPoint1->setText( red );
        display->setRoundResult(1, blue, red);

        ui->round1ovetime->setEnabled(true);
        ui->breaktimeButton->setEnabled(true);
    }
    else if ( ui->roundTwoButton->isChecked() ) {
        ui->bluePoint2->setText( blue );
        ui->redPoint2->setText( red );
        display->setRoundResult(2, blue, red);

        ui->round2ovetime->setEnabled(true);
        ui->breaktimeButton->setEnabled(true);
    }
    else if ( ui->roundThreeButton->isChecked() ) {
        ui->bluePoint3->setText( blue );
        ui->redPoint3->setText( red );
        display->setRoundResult(3, blue, red);

        ui->round3ovetime->setEnabled(true);
        ui->breaktimeButton->setEnabled(true);
    }
}

void FightControl::playSound(int index)
{
    player->setCurrentSource( *(sound.at(index)) );
    player->play();
}

void FightControl::roundChanged(int i )
{
    QString text;
    if ( i == 1 )
    {
        text = ui->roundOneButton->text();
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
        text = ui->roundTwoButton->text();
        ui->roundOneButton->setChecked(false);
        ui->round1ovetime->setChecked(false);
        ui->roundThreeButton->setChecked(false);
        ui->round3ovetime->setChecked(false);
        ui->breaktimeButton->setChecked(false);
        ui->breaktimeButton->setEnabled(false);
    }
    else if ( i == 3 )
    {
        text = ui->roundThreeButton->text();
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
        text = ui->breaktimeButton->text();

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
        if ( ui->styleCombo->currentIndex() == 2 ) {
            timerOrder = Ascendant;
            startMilliseconds = 0;
            stopMilliseconds = 2 * 60 * 1000;
        } else {
            timerOrder = Descendant;
            startMilliseconds = 2 * 60 * 1000;
            stopMilliseconds = 0;
        }
    } else {
        timerOrder = Descendant;
        startMilliseconds = 30 * 1000;
        stopMilliseconds = 0;
    }

    ui->startPauseButton->setEnabled(false);
    if ( ui->roundOneButton->isChecked()
         || ui->round1ovetime->isChecked()
         || ui->roundTwoButton->isChecked()
         || ui->round2ovetime->isChecked()
         || ui->roundThreeButton->isChecked()
         || ui->round3ovetime->isChecked()
         || ui->breaktimeButton->isChecked()
    ) {
        ui->startPauseButton->setEnabled(true);
    }

    setTimer();

    display->setRound(text);

    ui->bluePoint->setText(QString("0"));
    ui->redPoint->setText(QString("0"));

    display->setBluePoint(QString("0"));
    display->setRedPoint(QString("0"));
}

void FightControl::numEdit_changed(QString s)
{
    display->setNumberLabel(s);
}

void FightControl::styleCombo_changed(QString s)
{
    if ( ui->styleCombo->currentIndex() == 2 ) {
        timerOrder = Ascendant;
        startMilliseconds = 0;
        stopMilliseconds = 1.5 * 60 * 1000;
    } else {
        timerOrder = Descendant;
        startMilliseconds = 2 * 60 * 1000;
        stopMilliseconds = 0;
    }
    setTimer();
    display->setStyleLabel( QString("%1 %2 KG").arg(s).arg( ui->weightEdit->text() ) );
}

void FightControl::weightEdit_changed(QString w)
{
    display->setStyleLabel( QString("%1 %2 KG").arg( ui->styleCombo->currentText() ).arg( w ) );
}

void FightControl::redFlagButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Выберите файл флага"),
                                                    QCoreApplication::applicationDirPath(), QString("Svg (*.svg)") );
    redFlagWidget->load(filename);
    display->setRedFlag(filename);
}

void FightControl::clearRedFlag_clicked()
{
    redFlagWidget->load(QByteArray());
    display->setRedFlag(QString());
}

void FightControl::clearBlueFlag_clicked()
{
    blueFlagWidget->load(QByteArray());
    display->setBlueFlag(QString());

}

void FightControl::redCountry_changed()
{
    display->setRedCountry(ui->redCountryEdit->text());
}

void FightControl::redWrestler_changed()
{
    display->setRedWrestler(ui->redWrestlerEdit->text());
}

void FightControl::blueFlagButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Выберите файл флага"),
                                                    QCoreApplication::applicationDirPath(), QString("Svg (*.svg)") );
    blueFlagWidget->load(filename);
    display->setBlueFlag(filename);
}

void FightControl::blueCountry_changed()
{
    display->setBlueCountry(ui->blueCountryEdit->text());
}

void FightControl::blueWrestler_changed()
{
    display->setBlueWrestler(ui->blueWrestlerEdit->text());
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
}

void FightControl::setRedPoint(int p)
{
    int point = ui->redPoint->text().toInt();
    if ( 0 <= (point + p) )
        point += p;
    ui->redPoint->setText( QString::number(point) );
    display->setRedPoint( QString::number(point) );
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
