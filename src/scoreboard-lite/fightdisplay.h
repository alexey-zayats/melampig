#ifndef FIGHTDISPLAY_H
#define FIGHTDISPLAY_H

#include <QWidget>

class QSvgWidget;
class QBoxLayout;

#include <QStackedWidget>

namespace Ui {
    class FightDisplay;
    class Splash;
}

class FightDisplay : public QWidget
{
    Q_OBJECT

public:
    FightDisplay(QWidget *parent = 0);
    ~FightDisplay();

public slots:
    void setStyleLabel( const QString &label );
    void setNumberLabel( const QString &label );

    void setRedFlag(const QString &filename);
    void setRedCountry( const QString &label );
    void setRedWrestler( const QString &label );
    void setRedPoint(const QString &label);
    void setRedBan(int);
    void clearRedBan(int);

    void setBlueFlag(const QString &filename);
    void setBlueCountry( const QString &label );
    void setBlueWrestler( const QString &label );
    void setBluePoint(const QString &label);
    void setBlueBan(int);
    void clearBlueBan(int);

    void setRound(const QString &label);
    void setTimer(const QString &label);

    void setRoundResult(int num, const QString&, const QString&);
    void setBlueRoundResult(int num, const QString&);
    void setRedRoundResult(int num, const QString&);

    void showDisplay() { stack->setCurrentIndex(1); }
    void showSplash() { stack->setCurrentIndex(0); }

    void changeSettings();

protected slots:
    void keyPressEvent(QKeyEvent *e);

private:
    void initPalette();
    void setFontsSize();

    Ui::FightDisplay *displayUi;
    Ui::Splash *splashUi;

    QSvgWidget *redFlagWidget;
    QSvgWidget *blueFlagWidget;

    QPixmap *banPic;
    QPixmap *passPic;

    QStackedWidget *stack;
    QWidget *display;
    QWidget *splash;

    QBoxLayout *layout;
};

#endif // FIGHTDISPLAY_H
