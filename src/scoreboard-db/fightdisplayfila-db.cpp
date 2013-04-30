#include "fightdisplayfila-db.h"
#include "ui_fightdisplayfila-db.h"
#include "ui_splash-db.h"

#include <QtGui>
#include <QSvgWidget>
#include <QSettings>
#include <QDebug>

namespace Melampig
{
    FightDisplayFila::FightDisplayFila(QWidget *parent) :
        QWidget(parent),
        displayUi(new Ui::FightDisplayFilaDB),
        splashUi(new Ui::SplashDB)
    {
        setBlackPalette(this);

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

        bluePenPic = new QPixmap( ":icons/bullet_yellow_blue.png");
        redPenPic = new QPixmap( ":icons/bullet_yellow_red.png");
        blackPenPic = new QPixmap( ":icons/bullet_yellow_black.png");

        setFontsSize();

//        displayUi->redFirstLable1->setAutoFillBackground(true);
//        displayUi->blueFirstLable1->setAutoFillBackground(true);
//        displayUi->redFirstLable1->setVisible(true/false);
//        displayUi->redSecondLable1->setVisible(false);
//        displayUi->redThirdLable3->setVisible(false);

        numSign = displayUi->numLabel->text();
        displayUi->numLabel->setText( "");
    }

    void FightDisplayFila::setFontsSize()
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

        displayUi->redFirstLable1->setFont(font3);
        displayUi->redFirstLable2->setFont(font3);
        displayUi->redFirstLable3->setFont(font3);

        displayUi->redSecondLable1->setFont(font3);
        displayUi->redSecondLable2->setFont(font3);
        displayUi->redSecondLable3->setFont(font3);

        displayUi->redThirdLable1->setFont(font3);
        displayUi->redThirdLable2->setFont(font3);
        displayUi->redThirdLable3->setFont(font3);

        displayUi->blueFirstLable1->setFont(font3);
        displayUi->blueFirstLable2->setFont(font3);
        displayUi->blueFirstLable3->setFont(font3);

        displayUi->blueSecondLable1->setFont(font3);
        displayUi->blueSecondLable2->setFont(font3);
        displayUi->blueSecondLable3->setFont(font3);

        displayUi->blueThirdLable1->setFont(font3);
        displayUi->blueThirdLable2->setFont(font3);
        displayUi->blueThirdLable3->setFont(font3);

        displayUi->redPointLabel->setFont(font4);
        displayUi->bluePointLabel->setFont(font4);

        displayUi->numberLabel->setFont(font5);
        displayUi->numLabel->setFont(font5);

//        displayUi->penLabel1->setFont(font6);

        displayUi->styleWeightLable->setFont(font7);
        displayUi->rondNumLabel->setFont(font8);
        displayUi->timerLabel->setFont(font9);

        int x = settings.value("image-size/pen", 24).toInt();
        displayUi->redPen1->setMaximumSize(QSize(x,x));
        displayUi->redPen2->setMaximumSize(QSize(x,x));
        displayUi->redPen3->setMaximumSize(QSize(x,x));

        displayUi->bluePen1->setMaximumSize(QSize(x,x));
        displayUi->bluePen2->setMaximumSize(QSize(x,x));
        displayUi->bluePen3->setMaximumSize(QSize(x,x));

        redFlagWidget->setMaximumWidth(settings.value("flag/width", 200).toInt());
        redFlagWidget->setMaximumHeight(settings.value("flag/height", 100).toInt());

        redFlagWidget->setMinimumWidth(settings.value("flag/width", 200).toInt());
        redFlagWidget->setMinimumHeight(settings.value("flag/height", 100).toInt());

        blueFlagWidget->setMaximumWidth(settings.value("flag/width", 200).toInt());
        blueFlagWidget->setMaximumHeight(settings.value("flag/height", 100).toInt());

        blueFlagWidget->setMinimumWidth(settings.value("flag/width", 200).toInt());
        blueFlagWidget->setMinimumHeight(settings.value("flag/height", 100).toInt());
    }

    FightDisplayFila::~FightDisplayFila()
    {
        delete displayUi;
        delete splashUi;
    }

    void FightDisplayFila::setRoundWinner(int round, bool winner)
    {
        if ( round == 1 ) {

            if ( displayUi->redPen1->pixmap() != 0 ) {
                displayUi->redPen1->clear();
                displayUi->redFirstLable1->setPixmap( winner ? *redPenPic : *blackPenPic );
            }
            if ( displayUi->redPen2->pixmap() != 0 ) {
                displayUi->redPen2->clear();
                displayUi->redFirstLable2->setPixmap( winner ? *redPenPic : *blackPenPic );
            }
            if ( displayUi->redPen3->pixmap() != 0 ) {
                displayUi->redPen3->clear();
                displayUi->redFirstLable3->setPixmap( winner ? *redPenPic : *blackPenPic );
            }

            if ( displayUi->bluePen1->pixmap() != 0 ) {
                displayUi->bluePen1->clear();
                displayUi->blueFirstLable1->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }
            if ( displayUi->bluePen2->pixmap() != 0 ) {
                displayUi->bluePen2->clear();
                displayUi->blueFirstLable2->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }
            if ( displayUi->bluePen3->pixmap() != 0 ) {
                displayUi->bluePen3->clear();
                displayUi->blueFirstLable3->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }

            if ( winner ) { // red
                displayUi->redFirstLable1->setAutoFillBackground(true);
                if ( displayUi->redFirstLable1->pixmap() != 0 )
                    displayUi->redFirstLable1->setPixmap( *redPenPic );

                displayUi->redFirstLable2->setAutoFillBackground(true);
                if ( displayUi->redFirstLable2->pixmap() != 0 )
                    displayUi->redFirstLable2->setPixmap( *redPenPic );

                displayUi->redFirstLable3->setAutoFillBackground(true);
                if ( displayUi->redFirstLable3->pixmap() != 0 )
                    displayUi->redFirstLable3->setPixmap( *redPenPic );

                displayUi->blueFirstLable1->setAutoFillBackground(false);
                if ( displayUi->blueFirstLable1->pixmap() != 0 )
                    displayUi->blueFirstLable1->setPixmap( *blackPenPic );

                displayUi->blueFirstLable2->setAutoFillBackground(false);
                if ( displayUi->blueFirstLable2->pixmap() != 0 )
                    displayUi->blueFirstLable2->setPixmap( *blackPenPic );

                displayUi->blueFirstLable3->setAutoFillBackground(false);
                if ( displayUi->blueFirstLable3->pixmap() != 0 )
                    displayUi->blueFirstLable3->setPixmap( *blackPenPic );
            } else { // blue
                displayUi->blueFirstLable1->setAutoFillBackground(true);
                if ( displayUi->blueFirstLable1->pixmap() != 0 )
                    displayUi->blueFirstLable1->setPixmap( *bluePenPic );

                displayUi->blueFirstLable2->setAutoFillBackground(true);
                if ( displayUi->blueFirstLable2->pixmap() != 0 )
                    displayUi->blueFirstLable2->setPixmap( *bluePenPic );

                displayUi->blueFirstLable3->setAutoFillBackground(true);
                if ( displayUi->blueFirstLable3->pixmap() != 0 )
                    displayUi->blueFirstLable3->setPixmap( *bluePenPic );

                displayUi->redFirstLable1->setAutoFillBackground(false);
                if ( displayUi->redFirstLable1->pixmap() != 0 )
                    displayUi->redFirstLable1->setPixmap( *blackPenPic );

                displayUi->redFirstLable2->setAutoFillBackground(false);
                if ( displayUi->redFirstLable2->pixmap() != 0 )
                    displayUi->redFirstLable2->setPixmap( *blackPenPic );

                displayUi->redFirstLable3->setAutoFillBackground(false);
                if ( displayUi->redFirstLable3->pixmap() != 0 )
                    displayUi->redFirstLable3->setPixmap( *blackPenPic );
            }
        } else if ( round == 2 ) {

            if ( displayUi->redPen1->pixmap() != 0 ) {
                displayUi->redPen1->clear();
                displayUi->redSecondLable1->setPixmap( winner ? *redPenPic : *blackPenPic );
            }
            if ( displayUi->redPen2->pixmap() != 0 ) {
                displayUi->redPen2->clear();
                displayUi->redSecondLable2->setPixmap( winner ? *redPenPic : *blackPenPic );
            }
            if ( displayUi->redPen3->pixmap() != 0 ) {
                displayUi->redPen3->clear();
                displayUi->redSecondLable3->setPixmap( winner ? *redPenPic : *blackPenPic );
            }

            if ( displayUi->bluePen1->pixmap() != 0 ) {
                displayUi->bluePen1->clear();
                displayUi->blueSecondLable1->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }
            if ( displayUi->bluePen2->pixmap() != 0 ) {
                displayUi->bluePen2->clear();
                displayUi->blueSecondLable2->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }
            if ( displayUi->bluePen3->pixmap() != 0 ) {
                displayUi->bluePen3->clear();
                displayUi->blueSecondLable3->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }

            if ( winner ) { // red
                displayUi->redSecondLable1->setAutoFillBackground(true);
                if ( displayUi->redSecondLable1->pixmap() != 0 )
                    displayUi->redSecondLable1->setPixmap( *redPenPic );

                displayUi->redSecondLable2->setAutoFillBackground(true);
                if ( displayUi->redSecondLable2->pixmap() != 0 )
                    displayUi->redSecondLable2->setPixmap( *redPenPic );

                displayUi->redSecondLable3->setAutoFillBackground(true);
                if ( displayUi->redSecondLable3->pixmap() != 0 )
                    displayUi->redSecondLable3->setPixmap( *redPenPic );

                displayUi->blueSecondLable1->setAutoFillBackground(false);
                if ( displayUi->blueSecondLable1->pixmap() != 0 )
                    displayUi->blueSecondLable1->setPixmap( *blackPenPic );

                displayUi->blueSecondLable2->setAutoFillBackground(false);
                if ( displayUi->blueSecondLable2->pixmap() != 0 )
                    displayUi->blueSecondLable2->setPixmap( *blackPenPic );

                displayUi->blueSecondLable3->setAutoFillBackground(false);
                if ( displayUi->blueSecondLable3->pixmap() != 0 )
                    displayUi->blueSecondLable3->setPixmap( *blackPenPic );
            } else { // blue
                displayUi->blueSecondLable1->setAutoFillBackground(true);
                if ( displayUi->blueSecondLable1->pixmap() != 0 )
                    displayUi->blueSecondLable1->setPixmap( *bluePenPic );

                displayUi->blueSecondLable2->setAutoFillBackground(true);
                if ( displayUi->blueSecondLable2->pixmap() != 0 )
                    displayUi->blueSecondLable2->setPixmap( *bluePenPic );

                displayUi->blueSecondLable3->setAutoFillBackground(true);
                if ( displayUi->blueSecondLable3->pixmap() != 0 )
                    displayUi->blueSecondLable3->setPixmap( *bluePenPic );

                displayUi->redSecondLable1->setAutoFillBackground(false);
                if ( displayUi->redSecondLable1->pixmap() != 0 )
                    displayUi->redSecondLable1->setPixmap( *blackPenPic );

                displayUi->redSecondLable2->setAutoFillBackground(false);
                if ( displayUi->redSecondLable2->pixmap() != 0 )
                    displayUi->redSecondLable2->setPixmap( *blackPenPic );

                displayUi->redSecondLable3->setAutoFillBackground(false);
                if ( displayUi->redSecondLable3->pixmap() != 0 )
                    displayUi->redSecondLable3->setPixmap( *blackPenPic );
            }
        } else if ( round == 3 ) {

            if ( displayUi->redPen1->pixmap() != 0 ) {
                displayUi->redPen1->clear();
                displayUi->redThirdLable1->setPixmap( winner ? *redPenPic : *blackPenPic );
            }
            if ( displayUi->redPen2->pixmap() != 0 ) {
                displayUi->redPen2->clear();
                displayUi->redSecondLable2->setPixmap( winner ? *redPenPic : *blackPenPic );
            }
            if ( displayUi->redPen3->pixmap() != 0 ) {
                displayUi->redPen3->clear();
                displayUi->redSecondLable3->setPixmap( winner ? *redPenPic : *blackPenPic );
            }

            if ( displayUi->bluePen1->pixmap() != 0 ) {
                displayUi->bluePen1->clear();
                displayUi->blueSecondLable1->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }
            if ( displayUi->bluePen2->pixmap() != 0 ) {
                displayUi->bluePen2->clear();
                displayUi->blueSecondLable2->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }
            if ( displayUi->bluePen3->pixmap() != 0 ) {
                displayUi->bluePen3->clear();
                displayUi->blueSecondLable3->setPixmap( !winner ? *bluePenPic : *blackPenPic );
            }

            if ( winner ) { // red
                displayUi->redThirdLable1->setAutoFillBackground(true);
                if ( displayUi->redThirdLable1->pixmap() != 0 )
                    displayUi->redThirdLable1->setPixmap( *redPenPic );

                displayUi->redThirdLable2->setAutoFillBackground(true);
                if ( displayUi->redThirdLable2->pixmap() != 0 )
                    displayUi->redThirdLable2->setPixmap( *redPenPic );

                displayUi->redThirdLable3->setAutoFillBackground(true);
                if ( displayUi->redThirdLable3->pixmap() != 0 )
                    displayUi->redThirdLable3->setPixmap( *redPenPic );

                displayUi->blueThirdLable1->setAutoFillBackground(false);
                if ( displayUi->blueThirdLable1->pixmap() != 0 )
                    displayUi->blueThirdLable1->setPixmap( *blackPenPic );

                displayUi->blueThirdLable2->setAutoFillBackground(false);
                if ( displayUi->blueThirdLable2->pixmap() != 0 )
                    displayUi->blueThirdLable2->setPixmap( *blackPenPic );

                displayUi->blueThirdLable3->setAutoFillBackground(false);
                if ( displayUi->blueThirdLable3->pixmap() != 0 )
                    displayUi->blueThirdLable3->setPixmap( *blackPenPic );
            } else { // blue
                displayUi->blueThirdLable1->setAutoFillBackground(true);
                if ( displayUi->blueThirdLable1->pixmap() != 0 )
                    displayUi->blueThirdLable1->setPixmap( *bluePenPic );

                displayUi->blueThirdLable2->setAutoFillBackground(true);
                if ( displayUi->blueThirdLable2->pixmap() != 0 )
                    displayUi->blueThirdLable2->setPixmap( *bluePenPic );

                displayUi->blueThirdLable3->setAutoFillBackground(true);
                if ( displayUi->blueThirdLable3->pixmap() != 0 )
                    displayUi->blueThirdLable3->setPixmap( *bluePenPic );

                displayUi->redThirdLable1->setAutoFillBackground(false);
                if ( displayUi->redThirdLable1->pixmap() != 0 )
                    displayUi->redThirdLable1->setPixmap( *blackPenPic );

                displayUi->redThirdLable2->setAutoFillBackground(false);
                if ( displayUi->redThirdLable2->pixmap() != 0 )
                    displayUi->redThirdLable2->setPixmap( *blackPenPic );

                displayUi->redThirdLable3->setAutoFillBackground(false);
                if ( displayUi->redThirdLable3->pixmap() != 0 )
                    displayUi->redThirdLable3->setPixmap( *blackPenPic );
            }
        }
    }

    void FightDisplayFila::setRedBan(int /*pos*/)
    {
        if ( displayUi->redPen1->pixmap() == 0 )
            displayUi->redPen1->setPixmap( *redPenPic );
        else if ( displayUi->redPen2->pixmap() == 0  )
            displayUi->redPen2->setPixmap( *redPenPic );
        else if ( displayUi->redPen3->pixmap() == 0 )
            displayUi->redPen3->setPixmap( *redPenPic );
    }

    void FightDisplayFila::clearRedBan(int /*pos*/)
    {
        if ( displayUi->redPen3->pixmap() != 0 )
            displayUi->redPen3->clear();
        else if ( displayUi->redPen2->pixmap() != 0 )
            displayUi->redPen2->clear();
        else if ( displayUi->redPen1->pixmap() != 0 )
            displayUi->redPen1->clear();
    }

    void FightDisplayFila::setBlueBan(int /*pos*/)
    {
        if ( displayUi->bluePen1->pixmap() == 0 )
            displayUi->bluePen1->setPixmap( *bluePenPic );
        else if ( displayUi->bluePen2->pixmap() == 0  )
            displayUi->bluePen2->setPixmap( *bluePenPic );
        else if ( displayUi->bluePen3->pixmap() == 0 )
            displayUi->bluePen3->setPixmap( *bluePenPic );
    }

    void FightDisplayFila::clearBlueBan(int /*pos*/)
    {
        if ( displayUi->bluePen3->pixmap() != 0 )
            displayUi->bluePen3->clear();
        else if ( displayUi->bluePen2->pixmap() != 0 )
            displayUi->bluePen2->clear();
        else if ( displayUi->bluePen1->pixmap() != 0 )
            displayUi->bluePen1->clear();
    }

    void FightDisplayFila::clearFigthResult()
    {

        displayUi->redPen1->clear();
        displayUi->redPen2->clear();
        displayUi->redPen3->clear();

        displayUi->redFirstLable1->clear();
        displayUi->redFirstLable1->setAutoFillBackground(false);

        displayUi->redFirstLable2->clear();
        displayUi->redFirstLable2->setAutoFillBackground(false);

        displayUi->redFirstLable3->clear();
        displayUi->redFirstLable3->setAutoFillBackground(false);

        displayUi->redSecondLable1->clear();
        displayUi->redSecondLable1->setAutoFillBackground(false);

        displayUi->redSecondLable2->clear();
        displayUi->redSecondLable2->setAutoFillBackground(false);

        displayUi->redSecondLable3->clear();
        displayUi->redSecondLable3->setAutoFillBackground(false);

        displayUi->redThirdLable1->clear();
        displayUi->redThirdLable1->setAutoFillBackground(false);

        displayUi->redThirdLable2->clear();
        displayUi->redThirdLable2->setAutoFillBackground(false);

        displayUi->redThirdLable3->clear();
        displayUi->redThirdLable3->setAutoFillBackground(false);

        displayUi->bluePen1->clear();
        displayUi->bluePen2->clear();
        displayUi->bluePen3->clear();

        displayUi->blueFirstLable1->clear();
        displayUi->blueFirstLable1->setAutoFillBackground(false);

        displayUi->blueFirstLable2->clear();
        displayUi->blueFirstLable2->setAutoFillBackground(false);

        displayUi->blueFirstLable3->clear();
        displayUi->blueFirstLable3->setAutoFillBackground(false);

        displayUi->blueSecondLable1->clear();
        displayUi->blueSecondLable1->setAutoFillBackground(false);

        displayUi->blueSecondLable2->clear();
        displayUi->blueSecondLable2->setAutoFillBackground(false);

        displayUi->blueSecondLable3->clear();
        displayUi->blueSecondLable3->setAutoFillBackground(false);

        displayUi->blueThirdLable1->clear();
        displayUi->blueThirdLable1->setAutoFillBackground(false);

        displayUi->blueThirdLable2->clear();
        displayUi->blueThirdLable2->setAutoFillBackground(false);

        displayUi->blueThirdLable3->clear();
        displayUi->blueThirdLable3->setAutoFillBackground(false);
    }

    void FightDisplayFila::setRedPoint(const QString &label)
    {
        displayUi->redPointLabel->setText(label);
    }

    void FightDisplayFila::setBluePoint(const QString &label)
    {
        displayUi->bluePointLabel->setText(label);
    }

    void FightDisplayFila::setTimer(const QString &label)
    {
        displayUi->timerLabel->setText(label);
    }

    void FightDisplayFila::setRound(const QString &label)
    {
        displayUi->rondNumLabel->setText(label);
    }

    void FightDisplayFila::setRedWrestler( const QString &label )
    {
        displayUi->redWrestlerLabel->setText(label);
    }

    void FightDisplayFila::setBlueWrestler( const QString &label )
    {
        displayUi->blueWrestlerLabel->setText(label);
    }

    void FightDisplayFila::setRedCountry( const QString &label )
    {
        displayUi->redCountryLabel->setText(label);
    }

    void FightDisplayFila::setBlueCountry( const QString &label )
    {
        displayUi->blueCountryLabel->setText(label);
    }

    void FightDisplayFila::setStyleLabel( const QString &label )
    {
        displayUi->styleWeightLable->setText(label);
    }

    void FightDisplayFila::setNumberLabel( const QString &label )
    {
        displayUi->numLabel->setText( label.isEmpty() ? "" : numSign );
        displayUi->numberLabel->setText(label);
    }

    void FightDisplayFila::setRedFlag(const QByteArray &data)
    {
        if ( data.isEmpty() ) {
            redFlagWidget->hide();
            redFlagWidget->load(QByteArray());
        } else {
            redFlagWidget->show();
            redFlagWidget->load(data);
        }
    }

    void FightDisplayFila::setBlueFlag(const QByteArray &data)
    {
        if ( data.isEmpty() ) {
            blueFlagWidget->hide();
            blueFlagWidget->load(QByteArray());
        } else {
            blueFlagWidget->show();
            blueFlagWidget->load(data);
        }
    }

    void FightDisplayFila::keyPressEvent(QKeyEvent *e)
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

    void FightDisplayFila::setRedPalette(QWidget *widget)
    {
        QPalette palette3;

        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);

        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);

        QBrush brush2(QColor(255, 255, 220, 255));
        brush2.setStyle(Qt::SolidPattern);

        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush5(QColor(255, 0, 0, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush5);
        QBrush brush6(QColor(255, 127, 127, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush6);
        QBrush brush7(QColor(255, 63, 63, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush7);
        QBrush brush8(QColor(127, 0, 0, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush8);
        QBrush brush9(QColor(170, 0, 0, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush9);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush1);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush6);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
        widget->setPalette(palette3);
    }

    void FightDisplayFila::setBluePalette(QWidget *widget)
    {
        QPalette palette;

        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);

        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);

        QBrush brush2(QColor(255, 255, 220, 255));
        brush2.setStyle(Qt::SolidPattern);

        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush10(QColor(0, 0, 255, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush10);
        QBrush brush11(QColor(127, 127, 255, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush11);
        QBrush brush12(QColor(63, 63, 255, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush12);
        QBrush brush13(QColor(0, 0, 127, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush13);
        QBrush brush14(QColor(0, 0, 170, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush14);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush10);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush10);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush11);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush12);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush13);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush14);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush10);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush13);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush10);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush11);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush12);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush13);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush14);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush13);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush13);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush10);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
        widget->setPalette(palette);
    }

    void FightDisplayFila::setBlackPalette(QWidget *widget)
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
        widget->setPalette(palette);
    }

    void FightDisplayFila::changeSettings()
    {
        QSettings settings;
        splashUi->label->setPixmap(QPixmap( settings.value("splash", ":/logo-on-splash").toString() ));

        setFontsSize();
    }
}
