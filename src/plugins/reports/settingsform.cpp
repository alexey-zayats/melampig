#include "settingsform.h"
#include "ui_settingsform.h"

#include <orm.h>
#include <QSettings>
#include <QFileDialog>

#include <QDebug>

namespace Melampig
{

SettingsForm::SettingsForm(Keeper *keeer, QWidget *parent) :
    QWidget(parent),keeper(keeper),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(setOpenProgram()));
    connect(ui->folderButton, SIGNAL(clicked()), this, SLOT(setSaveFolder()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->useCompTitle, SIGNAL(clicked(bool)), this, SLOT(useCompTitle_checked(bool)));

    QSettings settings;
    settings.beginGroup("reports/settings");

    ui->openGroup->setChecked( settings.value("is_open", true).toBool() );
    ui->saveGroup->setChecked( settings.value("is_save", true).toBool() );

    bool ctafn = settings.value("comp_title_as_file_name", true).toBool();
    ui->useCompTitle->setChecked( ctafn );
    ui->fileLine->setEnabled(!ctafn);

#ifdef WIN32
    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\AcroExch.Document\\Shell\\Open\\Command",QSettings::NativeFormat);
    QString acroRdPath = reg.value("Default").toString();
    acroRdPath = acroRdPath.replace("%1", "");
    acroRdPath = acroRdPath.replace("\"", "");
    acroRdPath = acroRdPath.trimmed();

    if (!acroRdPath.isEmpty())
        ui->openLine->setText( settings.value("open_in", acroRdPath).toString()  );
#else
    ui->openLine->setText( settings.value("open_in").toString()  );
#endif

    QFileInfo aaa(QDir::tempPath());

    qDebug() << QDir::tempPath() << " is writeable? " <<  aaa.isWritable();
    ui->folderLine->setText( settings.value("save_folder", QDir::tempPath()).toString() );
    ui->fileLine->setText( settings.value("save_filename", "report.pdf").toString() );

    settings.endGroup();
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::useCompTitle_checked(bool check) {
    ui->fileLine->setEnabled(check);
}

void SettingsForm::setOpenProgram() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose program to open report PDF"),
                                                    QCoreApplication::applicationDirPath(), QString("Executable (*.exe)") );
    if ( !fileName.isEmpty() )
        ui->openLine->setText(fileName);
}

void SettingsForm::setSaveFolder() {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose forlder to store report PDF"),
                                                    QCoreApplication::applicationDirPath(),QFileDialog::ShowDirsOnly );
    if (!dirName.isEmpty())
        ui->folderLine->setText(dirName);
}

void SettingsForm::save() {
    QSettings settings;
    settings.beginGroup("reports/settings");

    settings.setValue("is_open",  ui->openGroup->isChecked() );
    settings.setValue("is_save",  ui->saveGroup->isChecked() );

    settings.setValue("open_in",   ui->openLine->text() );
    settings.setValue("save_folder",   ui->folderLine->text() );
    settings.setValue("save_filename",   ui->fileLine->text() );

    settings.setValue("comp_title_as_file_name",   ui->useCompTitle->isChecked() );

    settings.endGroup();
}


}
