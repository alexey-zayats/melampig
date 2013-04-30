#ifndef MELAMPIG_LOGONDIALOG_H
#define MELAMPIG_LOGONDIALOG_H

#include "ui_logondialog.h"
#include "exports.h"

namespace Melampig
{
	class MELAMPIG_UI_EXPORT LogonDialog : public QDialog
	{
		Q_OBJECT
		public:
			LogonDialog ( const QStringList &loginList, QWidget *parent = 0 );
			~LogonDialog();

			QString getLogin() const { return login; }
			QString getPassword() const { return password; }
			QStringList getLoginList() const;

		public slots:
			void on_okButton_clicked();
			void on_cancelButton_clicked();
			void setLogin( const QString &str ) { login = str; }
			void setPassword( const QString &str ) { password = str; }

		private:
			Ui::LogonDialog ui;
			QString login;
			QString password;
	};
}

#endif // MELAMPIG_LOGONDIALOG_H
