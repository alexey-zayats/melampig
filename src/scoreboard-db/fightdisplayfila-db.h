#ifndef FIGHTDISPLAYFILA_H
#define FIGHTDISPLAYFILA_H

#include <QWidget>

class QSvgWidget;
class QBoxLayout;

#include <QStackedWidget>

namespace Ui {
    class FightDisplayFilaDB;
    class SplashDB;
}

namespace Melampig
{
    class FightDisplayFila : public QWidget
    {
        Q_OBJECT

    public:
        FightDisplayFila(QWidget *parent = 0);
        ~FightDisplayFila();

    public slots:
        void setStyleLabel( const QString &label );
        void setNumberLabel( const QString &label );

        void setRedFlag(const QByteArray &data);
        void setRedCountry( const QString &label );
        void setRedWrestler( const QString &label );
        void setRedPoint(const QString &label);
        void setRedBan(int);
        void clearRedBan(int);

        void setBlueFlag(const QByteArray &data);
        void setBlueCountry( const QString &label );
        void setBlueWrestler( const QString &label );
        void setBluePoint(const QString &label);
        void setBlueBan(int);
        void clearBlueBan(int);

        void setRound(const QString &label);
        void setTimer(const QString &label);

        void setRoundWinner(int round, bool winner);

        void clearFigthResult();

        void showDisplay() { stack->setCurrentIndex(1); }
        void showSplash() { stack->setCurrentIndex(0); }

        void changeSettings();

    protected slots:
        void keyPressEvent(QKeyEvent *e);

    private:
        void setBlackPalette(QWidget *widget);
        void setBluePalette(QWidget *widget);
        void setRedPalette(QWidget *widget);

        void setFontsSize();

        Ui::FightDisplayFilaDB *displayUi;
        Ui::SplashDB *splashUi;

        QSvgWidget *redFlagWidget;
        QSvgWidget *blueFlagWidget;

        QPixmap *redPenPic;
        QPixmap *bluePenPic;
        QPixmap *blackPenPic;

        QStackedWidget *stack;
        QWidget *display;
        QWidget *splash;

        QBoxLayout *layout;

        QString numSign;
    };
}
#endif // FIGHTDISPLAY_H
