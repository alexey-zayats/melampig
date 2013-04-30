#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindowDB;
}

namespace Melampig
{
    class FightControl;
    class SettingsForm;
    class Keeper;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(Keeper *keeper, QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void actionSettings_triggered();
        void do_releaseFight();

    signals:
        void releaseFight();

    protected:
        void changeEvent(QEvent *e);
        void closeEvent(QCloseEvent *event);

    private:
        Ui::MainWindowDB *ui;
        Keeper *keeper;
        FightControl *control;
        SettingsForm *formSetting;
    };
}
#endif // MAINWINDOW_H
