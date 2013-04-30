#include <QtGui>

#include <projectdialog.h>
#include <project.h>
#include <logondialog.h>

#include "mainwindow.h"
#include "pluginbase.h"

namespace Melampig
{
	MainWindow::MainWindow( Project *pr, QWidget *parent )
	: QMainWindow( parent ), project(pr)
	{
		ui.setupUi(this);

		setObjectName ( "MainWindow" );

		workspace = new QWorkspace(this);
		workspace->setObjectName ( "main_workspace" );
		setCentralWidget(workspace);
		connect(workspace, SIGNAL(windowActivated(QWidget *)),
				this, SLOT(updateMenus()));

		windowMapper = new QSignalMapper(this);
		connect(windowMapper, SIGNAL(mapped(QWidget *)),
				workspace, SLOT(setActiveWindow(QWidget *)));

		connect(ui.menuWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

		connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
		connect(ui.actionClose, SIGNAL(triggered()), workspace, SLOT(closeActiveWindow()));
		connect(ui.actionClose_All, SIGNAL(triggered()), workspace, SLOT(closeAllWindows()));
		connect(ui.actionTile, SIGNAL(triggered()), workspace, SLOT(tile()));
		connect(ui.actionCascade, SIGNAL(triggered()), workspace, SLOT(cascade()));
		connect(ui.actionArrange_icons, SIGNAL(triggered()), workspace, SLOT(arrangeIcons()));
		connect(ui.action_Next, SIGNAL(triggered()), workspace, SLOT(activateNextWindow()));
		connect(ui.action_Previos, SIGNAL(triggered()), workspace, SLOT(activatePreviousWindow()));

		actionWindowMinimize = new QAction( tr("Minimize"), this );
		connect(actionWindowMinimize, SIGNAL(triggered()), this, SLOT(minimizeWindow()));
		actionWindowMaximize = new QAction( tr("Maximize"), this );
		connect(actionWindowMaximize, SIGNAL(triggered()), this, SLOT(maximizeWindow()));
		actionWindowClose = new QAction( tr("Close"), this );
		connect(actionWindowClose, SIGNAL(triggered()), this, SLOT(closeWindow()));

		tabBar = new QTabBar( this );
		tabBar->setExpanding(false);
		tabBar->setTabsClosable ( true );
		tabBar->setContextMenuPolicy( Qt::CustomContextMenu );

		connect( tabBar, SIGNAL( currentChanged ( int ) ), this, SLOT( tabBarChanged(int) ) );
		connect( tabBar, SIGNAL( customContextMenuRequested ( const QPoint & ) ), this, SLOT( tabContextMenu(const QPoint &) ) );
		connect( tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

		statusBar()->addWidget( tabBar, 100 );

		updateMenus();
		readSettings();

		loadPlugins();
		initPlugins();

		setWindowTitle(tr("Melampig") + ": " + project->getAlias() );
	}

	void MainWindow::closeEvent( QCloseEvent *e )
	{
		closeConnection();
		workspace->closeAllWindows();
		writeSettings();

		e->accept();
	}


	void MainWindow::readSettings()
	{
		QSettings settings;

		QPoint pos = settings.value("pos", QPoint(20, 20)).toPoint();
		QSize size = settings.value("size", QSize(800, 600)).toSize();

		move(pos);
		resize(size);
	}

	void MainWindow::writeSettings()
	{
		QSettings settings;
		settings.setValue("pos", pos());
		settings.setValue("size", size());
	}

	void MainWindow::loadPlugins()
	{
		QLibrary *lib = 0;
		PluginBase *pb = 0;
		QString pdir = QCoreApplication::applicationDirPath() + "/../lib";
		pluginFunction function;

		QDir dir( pdir );
		if ( !dir.exists() ) return;

		QFileInfoList list = dir.entryInfoList( QDir::Files | QDir::Readable | QDir::CaseSensitive, QDir::Name );
		for (int i = 0; i < list.size(); ++i)
		{
			QApplication::processEvents();
			QFileInfo fi = list.at(i);
			QString ext = fi.suffix();

			if( ext.compare(LIB_EXT) ) continue;
			lib = new QLibrary( pdir + "/" + fi.fileName() );

			if( !lib->load() ) {
				qDebug() << fi.fileName() << lib->errorString();
				continue;
			}

			function = (pluginFunction)lib->resolve("CreatePlugin");

			if( NULL == function ) continue;

			pb = function( workspace );
			pluginMap.insert( pb->name(), pb );
		}
	}

	void MainWindow::initPlugins()
	{
		PluginBase *plugin;
		QAction *lastAction = ui.menuWindow->menuAction();
		for ( TPluginMap::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it )
		{
			plugin = it.value();

			QToolBar *pluginToolBar = new QToolBar( plugin->caption(), this );
			pluginToolBar->setObjectName( plugin->name() );
			pluginToolBar->setIconSize ( QSize(16,16) );
			pluginToolBar->setWindowIcon( plugin->icon( ) );
			pluginToolBar->setMovable( false );
			pluginToolBar->addSeparator();

			QMenu *pluginMenu = new QMenu( plugin->caption(), this );
			ui.menuBar->insertMenu( lastAction, pluginMenu );
			pluginMenu->setObjectName( plugin->caption() );
			lastAction = pluginMenu->menuAction();

			for ( int i = 0; i < plugin->numCategories(); ++i )
			{
				QApplication::processEvents();

				QAction *a = pluginToolBar->addAction( plugin->categoryIcon( i ), plugin->categoryName( i ) );

				a->setData( QVariant(i) );
				a->setIconText( plugin->categoryName( i ) );
				a->setText( plugin->categoryName( i ) );

				pluginMenu->addAction( a );
			}
			addToolBar ( Qt::TopToolBarArea, pluginToolBar );
			connect( pluginToolBar, SIGNAL( actionTriggered (QAction*) ),
					this, SLOT( pluginActionTriggered(QAction*) ) );

			//if ( 4 < plugin->numCategories() )
			//	pluginToolBar->setVisible(false);
		}
	}

	void MainWindow::closeConnection()
	{
		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j )
			windows.at(j)->close();
		project->close();
	}

	void MainWindow::pluginActionTriggered(QAction*action)
	{
		PluginBase *plugin = 0;
		QString pluginName = action->parentWidget()->objectName();
		int id = action->data().toInt();

		for ( TPluginMap::Iterator it = pluginMap.begin(); it != pluginMap.end(); it++ )
		{
			if ( it.key() == pluginName )
				plugin = it.value();
		}

		if ( !plugin ) return;

		QString title = plugin->categoryName( id );

		QWidget* current = 0;
		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j )
			if ( windows.at(j)->windowTitle().compare( title ) == 0 )
				current = windows.at(j);

		if ( current )
			current->setFocus();
		else
		{
			current = plugin->setCurrentCategory( id, project->getKeeper(), centralWidget() );

			if ( !current ) return;

			current->setAttribute( Qt::WA_DeleteOnClose, true );
			current->setWindowTitle( title );
			current->setWindowIcon( plugin->categoryIcon( id ) );
			workspace->addWindow(current);
		}

		current->show();
	}

	void MainWindow::updateMenus()
	{
		bool hasChild = (activeChild() != 0);
		ui.actionClose->setEnabled(hasChild);
		ui.actionClose_All->setEnabled(hasChild);
		ui.actionTile->setEnabled(hasChild);
		ui.actionCascade->setEnabled(hasChild);
		ui.actionArrange_icons->setEnabled(hasChild);
		ui.action_Next->setEnabled(hasChild);
		ui.action_Previos->setEnabled(hasChild);

		QWidget *w = activeChild();
		if ( !w ) return;

		bool addNew = true;
		int i = 0;

		for( i = 0; i < tabBar->count(); i++ )
		{
			QApplication::processEvents();
			if ( tabBar->tabData(i).toString().compare(w->objectName()) == 0 ) {
				tabBar->setCurrentIndex(i);
				addNew = false;
			}
		}

		if ( addNew )
		{
			//tabBar->show();
			connect( w, SIGNAL( destroyed() ), this, SLOT( closeTab() ) );

			int tab_index = tabBar->addTab ( w->windowIcon(),  w->windowTitle() );
			tabBar->setTabData( tab_index,  w->objectName() );
			tabBar->setTabToolTip ( tab_index, w->windowTitle() );
			tabBar->setTabWhatsThis ( tab_index, w->windowTitle() );
			tabBar->setCurrentIndex(tab_index);

		}
	}

	void MainWindow::tabContextMenu(const QPoint &point)
	{
		int tabindex = tabBar->tabAt(point);
		currentWindow = tabBar->tabData(tabindex).toString();

		QMenu menu(this);
		menu.addAction( actionWindowMinimize );
		menu.addAction( actionWindowMaximize );
		menu.addAction( actionWindowClose );

		menu.exec( tabBar->mapToGlobal( point ) );
	}

	void MainWindow::tabBarChanged ( int idx )
	{
		QString title = tabBar->tabData( idx ).toString();
		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j ) {
			if ( windows.at(j)->objectName() == title ) {
				windows.at(j)->show();
				windows.at(j)->setFocus();
				return;
			}
		}
	}

	void MainWindow::minimizeWindow( )
	{
		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j ) {
			if ( windows.at(j)->objectName() == currentWindow ) {
				windows.at(j)->showMinimized ();
				return;
			}
		}
	}

	void MainWindow::maximizeWindow()
	{
		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j ) {
			if ( windows.at(j)->objectName() == currentWindow ) {
				windows.at(j)->showMaximized ();
				return;
			}
		}
	}

	void MainWindow::closeWindow()
	{
		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j ) {
			if ( windows.at(j)->objectName() == currentWindow ) {
				windows.at(j)->close ();
				return;
			}
		}
	}

	void MainWindow::updateWindowMenu()
	{
		ui.menuWindow->clear();
		ui.menuWindow->addAction(ui.actionClose);
		ui.menuWindow->addAction(ui.actionClose_All);
		ui.menuWindow->addSeparator();
		ui.menuWindow->addAction(ui.actionTile);
		ui.menuWindow->addAction(ui.actionCascade);
		ui.menuWindow->addAction(ui.actionArrange_icons);
		ui.menuWindow->addSeparator();
		ui.menuWindow->addAction(ui.action_Next);
		ui.menuWindow->addAction(ui.action_Previos);
		ui.menuWindow->addSeparator();

		QList<QWidget *> windows = workspace->windowList();
		for (int i = 0; i < windows.size(); ++i)
		{
			QWidget *child = qobject_cast<QWidget *>(windows.at(i));
			QAction *action  = ui.menuWindow->addAction( child->windowTitle() );
			action->setCheckable(true);
			action->setChecked( child == activeChild() );
			connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
			windowMapper->setMapping(action, child);
		}
	}

	QWidget *MainWindow::activeChild()
	{
		return qobject_cast<QWidget *>(workspace->activeWindow());
	}

	void MainWindow::closeTab(int index)
	{
		QVariant windowName = tabBar->tabData( index );
		tabBar->removeTab ( index );
		tabBar->update();

		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j ) {
			if ( windows.at(j)->objectName() == windowName.toString() ) {
				windows.at(j)->close ();
				return;
			}
		}
	}

	void MainWindow::closeTab()
	{
		QWidgetList windows = workspace->windowList();
		bool f;

		for( int i = 0; i < tabBar->count(); i++ ) {
			f = true;
			for ( int j = 0; j < windows.size(); ++j ) {
				if ( windows.at(j)->objectName() == tabBar->tabData(i).toString() ) {
					f = false;
					break;
				}
			}
			if ( f ) {
				tabBar->removeTab ( i );
				break;
			}
		}
		tabBar->update();
	}

	void MainWindow::on_actionAbout_Qt_triggered()
	{
		QApplication::aboutQt ();
	}
}
