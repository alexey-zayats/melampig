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
		ui(new Ui::ReferenceStack)
	{
		ui->setupUi(this);

		QHash<QString, ObjectType> refs;
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

		QStringList list;

		TQueryMap opt;
		ModelType mt;
		Object *o = 0;
		ObjectType ot;
		QWidget *widget = 0;
		QHash<QString, ObjectType>::iterator it = refs.begin();

		for( ; it != refs.end(); ++it )
		{
			list << it.key();

			ot = it.value();
			o = keeper->objectByType( ot );
			mt = o->getModelType();

			delete o;

			switch( mt )
			{
				case aList:  widget  = new ListWidget( ot, keeper, opt, parent ); break;
				case aTable: widget  = new TableWidget( ot, keeper, opt, parent ); break;
				case aTree:  widget  = new TreeWidget( ot, keeper, opt, parent ); break;
			}

			ui->stackedWidget->addWidget(widget);
		}

		model = new QStringListModel(list);
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
//		QString data = model->data(index, Qt::DisplayRole).toString();
		ui->stackedWidget->setCurrentIndex(row);
	}

}
