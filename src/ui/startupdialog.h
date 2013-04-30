// startupdialog.h

#ifndef MELAMPIG_STARTUPDAILOG_H
#define MELAMPIG_STARTUPDAILOG_H

#include "ui_startupdialog.h"
#include "exports.h"

namespace Melampig
{
	class Project;
	class MELAMPIG_UI_EXPORT StartUpDialog : public QDialog
	{
		Q_OBJECT
		public:
			StartUpDialog( QWidget *parent = 0 );
			~StartUpDialog();

			Project *getProject() { return project; }

		public slots:
			void on_runMnager_clicked();
			void on_addProject_clicked();
			void on_editProject_clicked();
			void on_deleteProject_clicked();
			void on_closeButton_clicked();

			void on_projectTree_itemSelectionChanged();
			void on_projectTree_itemDoubleClicked ( QTreeWidgetItem *, int );

		private:
			void initProjectList();
			Ui::StartUpDialog ui;
			Project *project;
	};
}

#endif // MELAMPIG_STARTUPDAILOG_H
