#include "referencestack.h"
#include "ui_referencestack.h"

#include <QHash>
#include <QStringListModel>

#include <ui.h>
#include <orm.h>

namespace Melampig
{
	ReferenceStack::ReferenceStack(Keeper *keeper, QWidget *parent) :
		QMainWindow(parent),
		keeper(keeper),
        ui(new Ui::ReferenceStack),
        parentWidget(parent)
	{
		ui->setupUi(this);

		refs.insert(tr("Action"), OAction);
		refs.insert(tr("Address type"), OAddressType);
		refs.insert(tr("Age"), OAge);
		refs.insert(tr("Circle"), OCircle);
		refs.insert(tr("Classify"), OClassify);
		refs.insert(tr("Coach"), OCoach);
		refs.insert(tr("Competition type"), OCompType);
		refs.insert(tr("Competition round"), OCRound);
		refs.insert(tr("Competition tour"), OCTour);
		refs.insert(tr("Fight rounds"), OFRound);
		refs.insert(tr("Gender"), OGender);
		refs.insert(tr("Geo"), OGeo);
		refs.insert(tr("Mat"), OMat);
		refs.insert(tr("Member"), OMember);
		refs.insert(tr("Position"), OPosition);
		refs.insert(tr("Rang"), ORang);
		refs.insert(tr("Referee"), OReferee);
		refs.insert(tr("Role"), ORole);
		refs.insert(tr("Style"), OStyle);
		refs.insert(tr("Team"), OTeam);
		refs.insert(tr("Weight"), OWeight);
		refs.insert(tr("Wrestler"), OWrestler);

        itemList = refs.keys();
        qSort(itemList);

        QWidget *w = 0;
        for( int i = 0; i < itemList.size(); i++ ) {
            w = new QWidget(this);
            w->setObjectName( QString("placeholder-%1").arg(i) );
            ui->stackedWidget->insertWidget(i, w);
        }

        model = new QStringListModel(itemList);
        ui->listView->setModel(model);

        ui->splitter->widget(0)->setMaximumWidth(200);

        connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(listView_activated(QModelIndex)));
        connect(ui->listView, SIGNAL(activated(QModelIndex)), this, SLOT(listView_activated(QModelIndex)));
	}

	ReferenceStack::~ReferenceStack()
	{
		delete ui;
	}

	void ReferenceStack::listView_activated(QModelIndex index)
	{
        if ( !index.isValid() ) return;

        int row = index.row();

        QWidget *w = ui->stackedWidget->widget(row);

        if ( w->objectName().compare(QString("placeholder-%1").arg(row)) == 0 )
        {
            ui->stackedWidget->removeWidget(w);
            delete w;

            TQueryMap opt;


            ObjectType ot = refs[itemList.at(row)];
            Object     *o = keeper->objectByType( ot );
            ModelType  mt = o->getModelType();
            delete o;

            switch( mt )
            {
                case aList:  w  = new ListWidget( ot, keeper, opt, parentWidget ); break;
                case aTable: w  = new TableWidget( ot, keeper, opt, parentWidget ); break;
                case aTree:  w  = new TreeWidget( ot, keeper, opt, parentWidget ); break;
            }

            ui->stackedWidget->insertWidget(row, w);
        }

        ui->stackedWidget->setCurrentIndex(row);
	}

}
