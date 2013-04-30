#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class FightControl;
class SettingsForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void actionSettings_triggered();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    FightControl *control;
    SettingsForm *formSetting;
};

#endif // MAINWINDOW_H
