#ifndef FIGHTCONTROL_H
#define FIGHTCONTROL_H

#include <QWidget>
#include <QHash>

class QSvgWidget;
class QSignalMapper;
class QLineEdit;
class QCheckBox;

namespace Ui {
    class FightControlDB;
}

class QTimer;

namespace Phonon {
    class MediaObject;
    class MediaSource;
}

namespace Melampig
{

//    class FightDisplay;
    class FightDisplayFila;

    class Object;
    class Keeper;
    class Wrestler;
    class Fight;

    class FightControl : public QWidget
    {
        Q_OBJECT

        enum TimerOrder {
            Ascendant,
            Descendant
        };

    protected:
        void keyPressEvent ( QKeyEvent * event );

    public:
        FightControl(Keeper *k, QWidget *parent = 0);
        ~FightControl();

    public slots:
        void openMonitor_clicked();
        void roundChanged(int);

        void stopButton_clicked();
        void startPauseBatton_toggled(bool);

        void timeOut();

        void increaseTimer();
        void decreaseTimer();

        void redPlusPoint_clicked();
        void redMinusPoint_clicked();

        void redPlusBan_clicked();
        void redMinusBan_clicked();

        void redPlusPoint1_clicked();
        void redPlusPoint2_clicked();
        void redPlusPoint3_clicked();
        void redPlusPoint5_clicked();

        void bluePlusPoint_clicked();
        void blueMinusPoint_clicked();

        void bluePlusBan_clicked();
        void blueMinusBan_clicked();

        void bluePlusPoint1_clicked();
        void bluePlusPoint2_clicked();
        void bluePlusPoint3_clicked();
        void bluePlusPoint5_clicked();

        void endofmatchButton_pressed();

        void roundOvertime_pressed(int);

        void showDisplay();
        void showSplash();

        void playSound(int);

        void plusRedPoint(int);
        void minusRedPoint(int);

        void plusBluePoint(int);
        void minusBluePoint(int);

        void changeSettings();

        void selectMatch_clicked();
        void do_releaseFight();

        void setRedRoundWinner(int);
        void setBlueRoundWinner(int);

    signals:
        void settingsChanged();

    private:
        void setRedPoint(int p);
        void setBluePoint(int p);
        void setTimer();

        void initSound();
        void initPointManage();
        void initRoundButtons();
        void initButtons();

        int currentRound();

        void initButtonStates();

        Ui::FightControlDB *ui;

//        FightDisplay *display;
        FightDisplayFila *display;

        QSvgWidget *redFlagWidget;
        QSvgWidget *blueFlagWidget;

        int startMilliseconds;
        int stopMilliseconds;

        QTimer *countdown;
        TimerOrder timerOrder;

        Phonon::MediaObject *player;
        QList<Phonon::MediaSource *> sound;

        QList<QLineEdit*> redPoints;
        QList<QLineEdit*> bluePoints;

        QList<QCheckBox*> redWin;
        QList<QCheckBox*> blueWin;

        int lastRound;

        Wrestler *red;
        Wrestler *blue;

        Keeper *keeper;
        Fight *object;

        QHash<QString,int> actions;
        QHash<QString,QStringList> rounds;

        QList<Object*> pendingActions;

        int roundnum,
            roundtime,
            overtime,
            resttime;

        bool overtimedirection,
              roundtimedirection,
              resttimedirection;

        bool isGR_18052013;
    };

}
#endif // FIGHTCONTROLE_H
