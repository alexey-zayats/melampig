#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

namespace Phonon {
    class MediaObject;
}

namespace Ui {
    class SettingsFormDB;
}

namespace Melampig
{
    class SettingsForm : public QWidget
    {
        Q_OBJECT

    public:
        explicit SettingsForm(QWidget *parent = 0);
        ~SettingsForm();

    public slots:
        void findSplash();
        void showSplash();

        void findEndRound();
        void playEndRound();

        void findLat30();
        void playLast30();

        void findSound1();
        void playSound1();

        void findSound2();
        void playSound2();

    signals:
        void settingsChanged();

    private slots:
        void save();

    private:
        void play(const QString &fileName);


        Ui::SettingsFormDB *ui;
        Phonon::MediaObject *player;
    };
}
#endif // SETTINGSFORM_H
