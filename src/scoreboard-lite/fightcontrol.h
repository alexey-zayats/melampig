#ifndef FIGHTCONTROL_H
#define FIGHTCONTROL_H

#include <QWidget>

class QSvgWidget;
class QSignalMapper;
class QLineEdit;

namespace Ui {
    class FightControl;
}

class FightDisplay;
class QTimer;

namespace Phonon {
    class MediaObject;
    class MediaSource;
}

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
    FightControl(QWidget *parent = 0);
    ~FightControl();

public slots:
    void redFlagButton_clicked();
    void redCountry_changed();
    void redWrestler_changed();

    void blueFlagButton_clicked();
    void blueCountry_changed();
    void blueWrestler_changed();

    void openMonitor_clicked();
    void styleCombo_changed(QString);
    void numEdit_changed(QString);
    void weightEdit_changed(QString);

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

    void clearRedFlag_clicked();
    void clearBlueFlag_clicked();

    void changeSettings();

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

    void initButtonStates();

    Ui::FightControl *ui;

    FightDisplay *display;

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
    int lastRound;
};

#endif // FIGHTCONTROLE_H
