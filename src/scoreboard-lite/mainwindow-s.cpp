#include "mainwindow-s.h"
#include "ui_mainwindow-s.h"
#include "fightcontrol.h"
#include "settingsform.h"

#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle( tr("Scoreborad") );

    control = new FightControl(this);
    control->setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(actionSettings_triggered()));

    setCentralWidget(control);
}

MainWindow::~MainWindow()
{
    delete ui;
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

