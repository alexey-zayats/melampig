#ifndef MELAMPIG_PROJECTDIALOG_H
#define MELAMPIG_PROJECTDIALOG_H

#include "ui_projectdialog.h"
#include "project.h"
#include "exports.h"

namespace Melampig
{
	class MELAMPIG_UI_EXPORT ProjectDialog : public QDialog
	{
		Q_OBJECT
		public:
			ProjectDialog ( const QString &host, const QString &dbname, QWidget *parent = 0 );
			ProjectDialog ( QWidget *parent = 0 );
			~ProjectDialog();

		public slots:
			void on_testButton_clicked();
			void on_okButton_clicked();
			void on_cancelButton_clicked();

		private:
			void setConnections();
			Ui::ProjectDialog ui;
			Project *project;
	};
}

#endif // MELAMPIG_PROJECTDIALOG_H
