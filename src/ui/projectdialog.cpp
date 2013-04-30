#include "projectdialog.h"
#include "logondialog.h"


#include <QMessageBox>
#include <QErrorMessage>
#include <errors.h>

namespace Melampig
{
	ProjectDialog::ProjectDialog( const QString &host, const QString &alias, QWidget *parent )
	: QDialog( parent )
	{
		ui.setupUi(this);
		project = new Project( host, alias );
		ui.dbEdit->setText( project->getName() );
		ui.aliasEdit->setText( project->getAlias() );
		ui.hostEdit->setText( project->getHost() );
		ui.portSpin->setValue( project->getPort() );
		ui.timeoutSpin->setValue( project->getTimeout() );
		ui.sslCombo->setCurrentIndex( project->getSSLMode() );
		setConnections();
	}

	ProjectDialog::ProjectDialog( QWidget *parent )
	: QDialog( parent )
	{
		ui.setupUi(this);
		project = new Project();
		setConnections();
	}

	ProjectDialog::~ProjectDialog( )
	{
		delete project;
	}

	void ProjectDialog::setConnections()
	{
		connect( ui.dbEdit, SIGNAL( textChanged (const QString &) ), project, SLOT( setName(const QString&) ) );
		connect( ui.aliasEdit, SIGNAL( textChanged (const QString &) ), project, SLOT( setAlias(const QString&) ) );
		connect( ui.hostEdit, SIGNAL( textChanged (const QString &) ), project, SLOT( setHost(const QString&) ) );
		connect( ui.portSpin, SIGNAL( valueChanged (int) ), project, SLOT( setPort(int) ) );
		connect( ui.timeoutSpin, SIGNAL( valueChanged (int) ), project, SLOT( setTimeout(int) ) );
		connect( ui.sslCombo, SIGNAL( currentIndexChanged(int) ), project, SLOT( setSSLMode(int) ) );
	}

	void ProjectDialog::on_testButton_clicked()
	{
		if ( ui.hostEdit->text().isEmpty() || ui.dbEdit->text().isEmpty() ) {
			QString msg = tr("Please fill fields: ") + QString("'%1' and '%2'").arg( tr("Hostname") ).arg( tr("Database") );
			QMessageBox::information( this, tr("Empty fields"), msg );
			return;
		}

		if ( ui.aliasEdit->text().isEmpty() ) {
			ui.aliasEdit->setText( QString("%1@%2:%3").arg(ui.dbEdit->text()).arg(ui.hostEdit->text()).arg(ui.portSpin->text()) );
		}

		const QStringList empty_string_list;

		LogonDialog ld( empty_string_list, this );
		if ( ld.exec() == QDialog::Accepted )
		{
			try
			{
				project->setUserName( ld.getLogin() );
				project->setPassword( ld.getPassword() );
				project->open();
				QMessageBox::information( this, tr("Information"), tr("Connection is ok") );
				ui.okButton->setDisabled( false );
			}
			catch ( SqlError &er )
			{
				QMessageBox::critical( this, tr("Error"), er.getText() + "\n" + er.getQuery() );
			}
			project->close();
		}
	}

	void ProjectDialog::on_okButton_clicked()
	{
		project->store();
		accept();
	}

	void ProjectDialog::on_cancelButton_clicked()
	{
		reject();
	}

}
