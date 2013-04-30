#include "mainwindow-db.h"
#include "ui_mainwindow-db.h"
#include "fightcontrol-db.h"
#include "settingsform-db.h"

#include "keeper.h"

#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>

namespace Melampig
{
    MainWindow::MainWindow(Keeper *keeper, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindowDB),
        keeper(keeper)
    {
        ui->setupUi(this);

        setWindowTitle( tr("Scoreborad") );

        control = new FightControl(keeper, this);
        control->setAttribute(Qt::WA_DeleteOnClose);

        connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(actionSettings_triggered()));
        connect(this, SIGNAL(releaseFight()), control, SLOT(do_releaseFight()));

        setCentralWidget(control);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::closeEvent(QCloseEvent * /*event*/ )
    {
        emit releaseFight();
        keeper->close();
    }

    void MainWindow::do_releaseFight()
    {
        emit releaseFight();
    }

    void MainWindow::changeEvent(QEvent *e)
    {
        QMainWindow::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    }

    void MainWindow::actionSettings_triggered()
    {
        formSetting = new SettingsForm();
        formSetting->setAttribute(Qt::WA_DeleteOnClose);
        formSetting->show();

        connect(formSetting, SIGNAL(settingsChanged()), control, SLOT(changeSettings()));
    }
}
