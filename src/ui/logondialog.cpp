#include "logondialog.h"

namespace Melampig
{
	LogonDialog::LogonDialog( const QStringList &list, QWidget *parent )
	: QDialog( parent )
	{
		ui.setupUi(this);

		for ( int i = 0; i < list.size(); i++ ) {
			ui.loginEdit->addItem ( list.at(i) );
		}

		setLogin( ui.loginEdit->currentText() );
		setPassword( ui.passwordEdit->text() );

		connect( ui.loginEdit, SIGNAL( editTextChanged (const QString &) ), this, SLOT( setLogin(const QString&) ) );
		connect( ui.passwordEdit, SIGNAL( textChanged (const QString &) ), this, SLOT( setPassword(const QString&) ) );

		ui.loginEdit->setInsertPolicy ( QComboBox::InsertAtTop );
	}

	LogonDialog::~LogonDialog( )
	{
	}

	QStringList LogonDialog::getLoginList() const
	{
		QStringList list;
		
		list.append( ui.loginEdit->currentText() );

		for( int i = 0; i < ui.loginEdit->count(); i++ )
			if ( !list.contains( ui.loginEdit->itemText(i) ) )
				list.append( ui.loginEdit->itemText(i) );
		
		return list;
	}
	
	void LogonDialog::on_okButton_clicked()
	{
		accept();
	}

	void LogonDialog::on_cancelButton_clicked()
	{
		reject();
	}
	
}
