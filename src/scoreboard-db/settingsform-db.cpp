#include "settingsform-db.h"
#include "ui_settingsform-db.h"

#include <QSettings>
#include <QFileDialog>
#include <QLabel>
#include <QBoxLayout>
#include <phonon>

namespace Melampig
{
    SettingsForm::SettingsForm(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SettingsFormDB)
    {
        ui->setupUi(this);

        connect(ui->splashFind, SIGNAL(clicked()), this, SLOT(findSplash()));
        connect(ui->splashShow, SIGNAL(clicked()), this, SLOT(showSplash()));

        connect(ui->endRoundFind, SIGNAL(clicked()), this, SLOT(findEndRound()));
        connect(ui->endRoundPlay, SIGNAL(clicked()), this, SLOT(playEndRound()));

        connect(ui->last30Find, SIGNAL(clicked()), this, SLOT(findLat30()));
        connect(ui->last30Play, SIGNAL(clicked()), this, SLOT(playLast30()));

        connect(ui->sound1Find, SIGNAL(clicked()), this, SLOT(findSound1()));
        connect(ui->sound1Play, SIGNAL(clicked()), this, SLOT(playSound1()));

        connect(ui->sound2Find, SIGNAL(clicked()), this, SLOT(findSound2()));
        connect(ui->sound2Play, SIGNAL(clicked()), this, SLOT(playSound2()));

        connect(ui->okButton, SIGNAL(clicked()), this, SLOT(save()));
        connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));

        QSettings settings;
        ui->endRoundEdit->setText(settings.value("sound/001").toString());
        ui->last30Edit->setText(settings.value("sound/002").toString());
        ui->sound1Edit->setText(settings.value("sound/003").toString());
        ui->sound2Edit->setText(settings.value("sound/004").toString());

        ui->splashEdit->setText(settings.value("splash").toString());

        ui->countrySpin->setValue( settings.value("font-size/country", 40).toInt() );
        ui->wrestlerSpin->setValue( settings.value("font-size/wrestler", 35).toInt() );
        ui->roundPointSpin->setValue( settings.value("font-size/round-point", 30).toInt() );
        ui->colorPointSpin->setValue( settings.value("font-size/color-point", 80).toInt() );
        ui->fightNumSpin->setValue( settings.value("font-size/fight-num", 60).toInt() );
        ui->penTextSpin->setValue( settings.value("font-size/pen", 20).toInt() );
        ui->styleSpin->setValue( settings.value("font-size/style", 90).toInt() );
        ui->roundNumSpin->setValue( settings.value("font-size/round-num", 30).toInt() );
        ui->timerSpin->setValue( settings.value("font-size/timer", 60).toInt() );
        ui->penImgSpin->setValue( settings.value("image-size/pen", 24).toInt() );

        ui->flagWidth->setValue(settings.value("flag/width", 200).toInt());
        ui->flagHeight->setValue(settings.value("flag/height", 100).toInt());

        player = Phonon::createPlayer(Phonon::MusicCategory);
    }

    SettingsForm::~SettingsForm()
    {
        delete ui;
    }


    void SettingsForm::findSplash()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Âûבונטעו פאיכ"),
                                                        QCoreApplication::applicationDirPath(), QString("Image (*.png *.gif *.jpeg *jpg)") );
        ui->splashEdit->setText(fileName);
    }

    void SettingsForm::showSplash()
    {
        QString fileName = ui->splashEdit->text();

        if ( fileName.isEmpty() )
            return;

        if ( !QFile::exists(fileName) )
            return;

        QWidget *widget = new QWidget;
        widget->setAttribute(Qt::WA_DeleteOnClose);

        QLabel *label = new QLabel(widget);

        QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, widget);
        layout->addWidget(label);

        widget->setLayout(layout);

        label->setPixmap( QPixmap( fileName ));
        label->adjustSize();

        widget->show();
    }

    void SettingsForm::findEndRound()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Âûבונטעו פאיכ"),
                                                        QCoreApplication::applicationDirPath(), QString("Sound (*.wav *.ogg *.mp3)") );
        ui->endRoundEdit->setText(fileName);
    }

    void SettingsForm::playEndRound()
    {
        play ( ui->endRoundEdit->text() );
    }

    void SettingsForm::findLat30()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Âûבונטעו פאיכ"),
                                                        QCoreApplication::applicationDirPath(), QString("Sound (*.wav *.ogg *.mp3)") );
        ui->last30Edit->setText(fileName);

    }

    void SettingsForm::playLast30()
    {
        play ( ui->last30Edit->text() );
    }

    void SettingsForm::findSound1()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Âûבונטעו פאיכ"),
                                                        QCoreApplication::applicationDirPath(), QString("Sound (*.wav *.ogg *.mp3)") );
        ui->sound1Edit->setText(fileName);
    }

    void SettingsForm::playSound1()
    {
        play ( ui->sound1Edit->text() );
    }

    void SettingsForm::findSound2()
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Âûבונטעו פאיכ"),
                                                        QCoreApplication::applicationDirPath(), QString("Sound (*.wav *.ogg *.mp3)") );
        ui->sound2Edit->setText(fileName);
    }

    void SettingsForm::playSound2()
    {
        play ( ui->sound2Edit->text() );
    }

    void SettingsForm::play(const QString &fileName)
    {

        if ( fileName.isEmpty() ) {
            return;
        }

        if ( !QFile::exists(fileName) ) {
            return;
        }

        Phonon::MediaSource mediaSource(fileName);

        player->setCurrentSource(mediaSource);
        player->play();
    }

    void SettingsForm::save()
    {
        QString endRound = ui->endRoundEdit->text();
        QString last30 = ui->last30Edit->text();
        QString sound1 = ui->sound1Edit->text();
        QString sound2 = ui->sound2Edit->text();

        QString splash = ui->splashEdit->text();

        QSettings settings;

        if ( QFile::exists(endRound) )
            settings.setValue("sound/001", endRound);
        else
            settings.remove("sound/001");

        if ( QFile::exists(last30) )
            settings.setValue("sound/002", last30);
        else
            settings.remove("sound/002");

        if ( QFile::exists(sound1) )
            settings.setValue("sound/003", sound1);
        else
            settings.remove("sound/003");

        if ( QFile::exists(sound2) )
            settings.setValue("sound/004", sound2);
        else
            settings.remove("sound/004");

        if ( QFile::exists(splash) )
            settings.setValue("splash", splash);
        else
            settings.remove("splash");

        settings.setValue("font-size/country", ui->countrySpin->value());
        settings.setValue("font-size/wrestler", ui->wrestlerSpin->value());
        settings.setValue("font-size/round-point", ui->roundPointSpin->value());
        settings.setValue("font-size/color-point", ui->colorPointSpin->value());
        settings.setValue("font-size/fight-num", ui->fightNumSpin->value());
        settings.setValue("font-size/pen", ui->penTextSpin->value());
        settings.setValue("font-size/style", ui->styleSpin->value());
        settings.setValue("font-size/round-num", ui->roundNumSpin->value());
        settings.setValue("font-size/timer", ui->timerSpin->value());
        settings.setValue("image-size/pen", ui->penImgSpin->value());

        settings.setValue("flag/width", ui->flagWidth->value());
        settings.setValue("flag/height", ui->flagHeight->value());

        emit settingsChanged();

        //close();
    }

}

// sound/001 - end round
// sound/002 - first
// sound/003 - second
// sound/004 - 1:30 GR
// splash
