#include "winnerdialog-db.h"
#include "ui_winnerdialog-db.h"

#include <orm.h>

#include <QMessageBox>

namespace Melampig
{
    WinnerDialog::WinnerDialog(Object *o, Keeper *k, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::WinnerDialogDB),
        object(o),
        keeper(k)
    {
        ui->setupUi(this);

        TQueryMap opt;
        QList<QVariant> list = keeper->getListOfFields(OClassify, QStringList() << "title" << "id", opt );

        ui->classifyCombo->setMaximumWidth(300);
        ui->classifyCombo->addItem( tr("Select classify points"), QString("0") );
        for(int i = 0; i < list.size(); i++) {
            QStringList row = list.at(i).toStringList();
            ui->classifyCombo->addItem( QString("%1").arg(row.at(0)), row.at(1) );
        }
        connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));

        ui->okButton->setEnabled(false);

        connect(ui->blueRadio, SIGNAL(clicked()), this, SLOT(checkOkButton()));
        connect(ui->redRadio, SIGNAL(clicked()), this, SLOT(checkOkButton()));
        connect(ui->classifyCombo, SIGNAL(activated(int)), this, SLOT(checkOkButton()));
    }

    WinnerDialog::~WinnerDialog()
    {
        delete ui;
    }

    void WinnerDialog::checkOkButton()
    {
        if ((ui->blueRadio->isChecked() || ui->redRadio->isChecked()) && ui->classifyCombo->currentIndex() > 0 )
            ui->okButton->setEnabled(true);
        else
            ui->okButton->setEnabled(false);
    }

    void WinnerDialog::accept()
    {
        object->restore( object->get("id").toInt() );

        if ( (ui->blueRadio->isChecked() || ui->redRadio->isChecked() )
             && ui->classifyCombo->currentIndex() > 0 )
        {

            if ( ui->blueRadio->isChecked() )
                object->set("winner", object->get("blue"));
            else if ( ui->redRadio->isChecked() )
                object->set("winner", object->get("red"));


            object->set("classify",
                        ui->classifyCombo->itemData(
                            ui->classifyCombo->currentIndex(),
                            Qt::UserRole
                            ).toString()
                        );

        }

        object->set("on_fight", "false");

        try
        {
            object->store();
        } catch ( SqlError &err ) {
            QMessageBox::critical( this, tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        QDialog::accept();
    }

}
