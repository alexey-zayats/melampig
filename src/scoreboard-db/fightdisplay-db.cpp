#include "fightdisplay-db.h"
#include "ui_fightdisplay-db.h"
#include "ui_splash-db.h"

#include <QtGui>
#include <QSvgWidget>
#include <QSettings>
#include <QDebug>

namespace Melampig
{
    FightDisplay::FightDisplay(QWidget *parent) :
        QWidget(parent),
        displayUi(new Ui::FightDisplayDB),
        splashUi(new Ui::SplashDB)
    {
        initPalette();

        stack = new QStackedWidget(this);

        display = new QWidget(stack);
        splash  = new QWidget(stack);

        displayUi->setupUi(display);
        splashUi->setupUi(splash);

        stack->addWidget(splash);
        stack->addWidget(display);

        QSettings settings;
        splashUi->label->setPixmap(QPixmap( settings.value("splash", ":/logo-on-splash").toString() ));
        splashUi->label->setMaximumSize( QApplication::desktop()->screenGeometry().size() );
        splashUi->label->setScaledContents(true);

        layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
        layout->setMargin(0);
        layout->addWidget(stack);

        setWindowTitle( tr("Display") );

        displayUi->countryFlagLayout->removeWidget(displayUi->redFlagLabel);
        displayUi->redFlagLabel->deleteLater();

        displayUi->countryFlagLayout->removeWidget(displayUi->redCountryLabel);
        displayUi->countryFlagLayout->removeWidget(displayUi->blueCountryLabel);

        displayUi->countryFlagLayout->removeWidget(displayUi->blueFlagLabel);
        displayUi->blueFlagLabel->deleteLater();

        redFlagWidget = new QSvgWidget(display);
        redFlagWidget->setMaximumSize(400,200);
        redFlagWidget->hide();

        displayUi->countryFlagLayout->addWidget(redFlagWidget, 0, 0);
        displayUi->countryFlagLayout->addWidget(displayUi->redCountryLabel, 0, 1);

        blueFlagWidget = new QSvgWidget(display);
        blueFlagWidget->setMaximumSize(400,200);
        blueFlagWidget->hide();

        displayUi->countryFlagLayout->addWidget(displayUi->blueCountryLabel, 0, 2);
        displayUi->countryFlagLayout->addWidget(blueFlagWidget, 0, 3);

        banPic = new QPixmap( ":/record");
        passPic = new QPixmap( ":/remove");

        setFontsSize();
    }

    void FightDisplay::setFontsSize()
    {

        QSettings settings;

        QFont font1;
        font1.setPointSize(settings.value("font-size/country", 40).toInt());

        QFont font2;
        font2.setPointSize(settings.value("font-size/wrestler", 35).toInt());

        QFont font3;
        font3.setPointSize(settings.value("font-size/round-point", 30).toInt());

        QFont font4;
        font4.setPointSize(settings.value("font-size/color-point", 80).toInt());

        QFont font5;
        font5.setPointSize(settings.value("font-size/fight-num", 60).toInt());

        QFont font6;
        font6.setPointSize(settings.value("font-size/pen", 20).toInt());

        QFont font7;
        font7.setPointSize(settings.value("font-size/style", 90).toInt());

        QFont font8;
        font8.setPointSize(settings.value("font-size/round-num", 30).toInt());

        QFont font9;
        font9.setPointSize(settings.value("font-size/timer", 60).toInt());

        displayUi->redCountryLabel->setFont(font1);
        displayUi->blueCountryLabel->setFont(font1);

        displayUi->redWrestlerLabel->setFont(font2);
        displayUi->blueWrestlerLabel->setFont(font2);

        displayUi->redFirstLable->setFont(font3);
        displayUi->redSecoundLabel->setFont(font3);
        displayUi->redThirdLabel->setFont(font3);

        displayUi->blueFirstLabel->setFont(font3);
        displayUi->blueSecondLabel->setFont(font3);
        displayUi->blueThirdLabel->setFont(font3);

        displayUi->redPointLabel->setFont(font4);
        displayUi->bluePointLabel->setFont(font4);

        displayUi->numberLabel->setFont(font5);
        displayUi->numLabel->setFont(font5);

        displayUi->penLabel1->setFont(font6);

        displayUi->styleWeightLable->setFont(font7);

        displayUi->rondNumLabel->setFont(font8);

        displayUi->timerLabel->setFont(font9);


        int x = settings.value("image-size/pen", 24).toInt();
        displayUi->redBan1->setMaximumSize(QSize(x,x));
        displayUi->redBan2->setMaximumSize(QSize(x,x));
        displayUi->redBan3->setMaximumSize(QSize(x,x));

        displayUi->blueBan1->setMaximumSize(QSize(x,x));
        displayUi->blueBan2->setMaximumSize(QSize(x,x));
        displayUi->blueBan3->setMaximumSize(QSize(x,x));

        redFlagWidget->setMaximumWidth(settings.value("flag/width", 200).toInt());
        redFlagWidget->setMaximumHeight(settings.value("flag/height", 100).toInt());

        redFlagWidget->setMinimumWidth(settings.value("flag/width", 200).toInt());
        redFlagWidget->setMinimumHeight(settings.value("flag/height", 100).toInt());

        blueFlagWidget->setMaximumWidth(settings.value("flag/width", 200).toInt());
        blueFlagWidget->setMaximumHeight(settings.value("flag/height", 100).toInt());

        blueFlagWidget->setMinimumWidth(settings.value("flag/width", 200).toInt());
        blueFlagWidget->setMinimumHeight(settings.value("flag/height", 100).toInt());
    }

    FightDisplay::~FightDisplay()
    {
        delete displayUi;
        delete splashUi;
    }

    void FightDisplay::setRedBan(int pos)
    {
        if ( pos == 1 ) displayUi->redBan1->setPixmap( *banPic );
        else if ( pos == 2 ) displayUi->redBan2->setPixmap( *banPic );
        else if ( pos == 3 ) displayUi->redBan3->setPixmap( *banPic );
    }

    void FightDisplay::clearRedBan(int pos)
    {
        if ( pos == 1 ) displayUi->redBan1->clear();
        else if ( pos == 2 ) displayUi->redBan2->clear();
        else if ( pos == 3 ) displayUi->redBan3->clear();
    }

    void FightDisplay::setBlueBan(int pos)
    {
        if ( pos == 1 ) displayUi->blueBan1->setPixmap( *banPic );
        else if ( pos == 2 ) displayUi->blueBan2->setPixmap( *banPic );
        else if ( pos == 3 ) displayUi->blueBan3->setPixmap( *banPic );
    }

    void FightDisplay::clearBlueBan(int pos)
    {
        if ( pos == 1 ) displayUi->blueBan1->clear();
        else if ( pos == 2 ) displayUi->blueBan2->clear();
        else if ( pos == 3 ) displayUi->blueBan3->clear();
    }

    void FightDisplay::setRedRoundResult(int num, const QString &value)
    {
        if ( num == 1 ) {
            displayUi->redFirstLable->setText(value);
        } else if ( num == 2 ) {
            displayUi->redSecoundLabel->setText(value);
        } else if ( num ==3 ) {
            displayUi->redThirdLabel->setText(value);
        }
    }

    void FightDisplay::setBlueRoundResult(int num, const QString &value)
    {
        if ( num == 1 ) {
            displayUi->blueFirstLabel->setText(value);
        } else if ( num == 2 ) {
            displayUi->blueSecondLabel->setText(value);
        } else if ( num ==3 ) {
            displayUi->blueThirdLabel->setText(value);
        }
    }

    void FightDisplay::setRoundResult(int num, const QString&blue, const QString&red)
    {
        if ( num == 1 ) {
            displayUi->redFirstLable->setText(red);
            displayUi->blueFirstLabel->setText(blue);
        } else if ( num == 2 ) {
            displayUi->redSecoundLabel->setText(red);
            displayUi->blueSecondLabel->setText(blue);
        } else if ( num ==3 ) {
            displayUi->redThirdLabel->setText(red);
            displayUi->blueThirdLabel->setText(blue);
        }
    }

    void FightDisplay::setRedPoint(const QString &label)
    {
        displayUi->redPointLabel->setText(label);
    }

    void FightDisplay::setBluePoint(const QString &label)
    {
        displayUi->bluePointLabel->setText(label);
    }

    void FightDisplay::setTimer(const QString &label)
    {
        displayUi->timerLabel->setText(label);
    }

    void FightDisplay::setRound(const QString &label)
    {
        displayUi->rondNumLabel->setText(label);
    }

    void FightDisplay::setRedWrestler( const QString &label )
    {
        displayUi->redWrestlerLabel->setText(label);
    }

    void FightDisplay::setBlueWrestler( const QString &label )
    {
        displayUi->blueWrestlerLabel->setText(label);
    }

    void FightDisplay::setRedCountry( const QString &label )
    {
        displayUi->redCountryLabel->setText(label);
    }

    void FightDisplay::setBlueCountry( const QString &label )
    {
        displayUi->blueCountryLabel->setText(label);
    }

    void FightDisplay::setStyleLabel( const QString &label )
    {
        displayUi->styleWeightLable->setText(label);
    }

    void FightDisplay::setNumberLabel( const QString &label )
    {
        displayUi->numberLabel->setText(label);
    }

    void FightDisplay::setRedFlag(const QByteArray &data)
    {
        if ( data.isEmpty() ) {
            redFlagWidget->hide();
            redFlagWidget->load(QByteArray());
        } else {
            redFlagWidget->show();
            redFlagWidget->load(data);
        }
    }

    void FightDisplay::setBlueFlag(const QByteArray &data)
    {
        if ( data.isEmpty() ) {
            blueFlagWidget->hide();
            blueFlagWidget->load(QByteArray());
        } else {
            blueFlagWidget->show();
            blueFlagWidget->load(data);
        }
    }

    void FightDisplay::keyPressEvent(QKeyEvent *e)
    {
        QWidget::keyPressEvent(e);

        if ( e->key() == Qt::Key_F11 ) {
            Qt::WindowStates state = windowState();
            if ( state & Qt::WindowFullScreen )
                setWindowState( state ^ Qt::WindowFullScreen );
            else
                setWindowState( state | Qt::WindowFullScreen );
        } else if ( e->key() == Qt::Key_F12 ) {
            stack->setCurrentIndex( stack->currentIndex() == 0 ? 1 : 0 );
        }
    }


    void FightDisplay::initPalette()
    {
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush2(QColor(255, 255, 220, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
        setPalette(palette);
    }

    void FightDisplay::changeSettings()
    {
        QSettings settings;
        splashUi->label->setPixmap(QPixmap( settings.value("splash", ":/logo-on-splash").toString() ));

        setFontsSize();
    }
}
