#ifndef MELAMPIG_MAINWINDOW_H
#define MELAMPIG_MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QHash>

class QWorkspace;
class QSignalMapper;
class QAction;
class QMenu;
class QTabBar;

#ifdef Q_OS_WIN32
#define LIB_EXT "dll"
#else
#define LIB_EXT "so"
#endif

namespace Melampig
{
	class Project;
	class PluginBase;
	typedef PluginBase* (*pluginFunction)( QWidget * );
	typedef QHash<QString, PluginBase*> TPluginMap;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT
		public:
			MainWindow( Project *pr, QWidget *parent = 0 );

		protected:
			void closeEvent( QCloseEvent * );

		private slots:
			void updateMenus();
			void updateWindowMenu();

			void on_actionAbout_Qt_triggered();
			void closeTab();
			void closeTab(int);

			void pluginActionTriggered(QAction*);

			void tabBarChanged ( int );
			void tabContextMenu(const QPoint &);

			// slots for context menu action
			void minimizeWindow();
			void maximizeWindow();
			void closeWindow();

		private:
			void closeConnection();
			void readSettings();
			void writeSettings();
			void loadPlugins();
			void initPlugins();
			QWidget *activeChild();

			QTabBar *tabBar;

			Ui::MainWindow ui;
			QWorkspace *workspace;
			QSignalMapper *windowMapper;
			TPluginMap pluginMap;
			Project *project;

			QAction *actionWindowMinimize,
					*actionWindowMaximize,
					*actionWindowClose;

			QString currentWindow;
	};
}

#endif // MELAMPIG_MAINWINDOW_H
