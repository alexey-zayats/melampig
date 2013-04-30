#include <QApplication>
#include <QTextCodec>
#include <QPlastiqueStyle>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>

#include <orm.h>
#include "project.h"
#include "startupdialog.h"
#include "errors.h"

#include "mainwindow-db.h"

using namespace Melampig;

int main(int argc, char *argv[])
{
	QTextCodec *codec=QTextCodec::codecForName("UTF8");
	QTextCodec::setCodecForCStrings(codec);

	Q_INIT_RESOURCE(melampig);

    QApplication app(argc, argv);
    app.setOrganizationName("Agitum");
    app.setOrganizationDomain("agitum.ru");
    app.setApplicationName("Scoreboard.Lite");

    QTranslator *translator = new QTranslator();
    translator->load( "scoreboard_ru", app.applicationDirPath() + "/../share/translations" );
    app.installTranslator(translator);

    translator = new QTranslator();
    translator->load( "ui_ru", app.applicationDirPath() + "/../share/translations" );
    app.installTranslator(translator);


    translator = new QTranslator();
    translator->load( "qt_ru", app.applicationDirPath() + "/../share/translations" );
    app.installTranslator(translator);

	int retval = 0;
	StartUpDialog s(0);
	if ( s.exec() == QDialog::Accepted )
	{
		MainWindow *mw = new MainWindow( s.getProject()->getKeeper() );
		QObject::connect(mw, SIGNAL(destroyed()), mw, SLOT(do_releaseFight()));
		mw->show();
		try
		{
			retval = app.exec();
		} catch (Melampig::SqlError &err) {
			QMessageBox::critical( 0, QObject::tr("Error"), err.getText() + "\n" + err.getQuery() );
		} catch (...) {
			QMessageBox::critical( 0, QObject::tr("Error"), "Uncatchable programm error" );
		}
	}

	return retval;
}

