// startupdialog.cpp

#include <QHeaderView>
#include <QSettings>
#include <QMessageBox>

#include "errors.h"
#include "startupdialog.h"
#include "projectdialog.h"
#include "project.h"
#include "logondialog.h"

namespace Melampig
{
	StartUpDialog::StartUpDialog( QWidget *parent )
	: QDialog( parent )
	{
		ui.setupUi(this);
		project = 0;

		ui.projectTree->header()->hide();
		ui.projectTree->setColumnCount(3);
		ui.projectTree->header()->hideSection(1);
		ui.projectTree->header()->hideSection(2);

		initProjectList();
	}

	StartUpDialog::~StartUpDialog()
	{
	}

	void StartUpDialog::initProjectList()
	{
		ui.projectTree->clear();

		QSettings s;
		s.beginGroup("projects");

		QTreeWidgetItem *root, *item = 0;

		QStringList hosts = s.childGroups();
		for (int i = 0; i < hosts.size(); ++i)
		{
			root = new QTreeWidgetItem( ui.projectTree );
			root->setText( 0, hosts.at(i) );
            root->setIcon( 0, QIcon(QString::fromUtf8(":/icons/server.png")));
			root->setText( 1, "0" );

			s.beginGroup(hosts.at(i));

			QStringList aliases = s.childGroups();
			for (int j = 0; j < aliases.size(); ++j)
			{
				QApplication::processEvents();
				s.beginGroup( aliases.at(j) );
				item = new QTreeWidgetItem( root );
				item->setText( 0, aliases.at(j) );
                item->setIcon( 0, QIcon(QString::fromUtf8(":/icons/database.png")));
				item->setText( 1, "1" );
				ui.projectTree->expandItem(item);
				s.endGroup();
			}

			s.endGroup();

			ui.projectTree->expandItem(root);
		}
		s.endGroup();
	}

	void StartUpDialog::on_projectTree_itemSelectionChanged()
	{
		ui.runMnager->setDisabled( true );
		ui.editProject->setDisabled( true );
		ui.deleteProject->setDisabled( true );
		ui.dbInfo->clear();

		QList<QTreeWidgetItem *> list = ui.projectTree->selectedItems();
		if ( list.size() == 0 ) return;

		QTreeWidgetItem *item = list.first();
		if ( !item ) return;

		int type = item->text(1).toInt();

		if ( type == 0 ) return;
		ui.dbInfo->setText( item->text(0) );

		ui.runMnager->setDisabled( false );
		ui.editProject->setDisabled( false );
		ui.deleteProject->setDisabled( false );
	}

	void StartUpDialog::on_projectTree_itemDoubleClicked ( QTreeWidgetItem *item, int )
	{
		if ( !item ) return;
		int type = item->text(1).toInt();

		if ( type != 1 )  return;

		QString alias = item->text(0);
		QString host = item->parent()->text(0);

		if ( alias.isEmpty() ) return;

		project = new Project( host, alias, this );
		LogonDialog ld( project->getLoginList(), this );
		if ( ld.exec() == QDialog::Accepted  )
		{
			try
			{
				project->setUserName( ld.getLogin() );
				project->setPassword( ld.getPassword() );

				project->setLoginList( ld.getLoginList() );
				project->store();

				project->open();

				accept();
			}
			catch ( SqlError &er )
			{
				delete project;
				QMessageBox::critical( this, tr("Error"), er.getText() + "\n" + er.getQuery() );
//				reject();
			}
		}
	}

	void StartUpDialog::on_runMnager_clicked()
	{
		emit on_projectTree_itemDoubleClicked ( ui.projectTree->currentItem (), 0 );
	}

	void StartUpDialog::on_addProject_clicked()
	{
		ProjectDialog pd( this );
		if ( pd.exec() == QDialog::Accepted )
			initProjectList();
	}

	void StartUpDialog::on_editProject_clicked()
	{
		QTreeWidgetItem *item = ui.projectTree->currentItem();
		if ( !item ) return;

		QString alias = item->text(0);
		QString host = item->parent()->text(0);
		ProjectDialog pd( host, alias, this );
		if ( pd.exec() == QDialog::Accepted  )
			initProjectList();
	}

	void StartUpDialog::on_deleteProject_clicked()
	{
		QTreeWidgetItem *item = ui.projectTree->currentItem();
		if ( !item ) return;

		QString alias = item->text(0);
		QString host = item->parent()->text(0);

		int res = QMessageBox::question( this, tr("Question"), tr("Are you shure to remove selected project?"),
										 QMessageBox::Yes, QMessageBox::No );
		if ( res == QMessageBox::Yes )
		{
			Project p(host, alias, this );
			p.remove();
			initProjectList();
		}
	}

	void StartUpDialog::on_closeButton_clicked()
	{
		QDialog::reject();
	}

}
