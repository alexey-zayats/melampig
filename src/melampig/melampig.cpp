
#include <QApplication>
#include <QTextCodec>
#include <QPlastiqueStyle>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QSettings>

#include "startupdialog.h"
#include "mainwindow.h"
#include "errors.h"

using namespace Melampig;

int main(int argc, char *argv[])
{
	QTextCodec *codec=QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForCStrings(codec);

    Q_INIT_RESOURCE(melampig);

    QApplication app(argc, argv);

    app.setOrganizationName("Agitum");
    app.setOrganizationDomain("agitum.ru");
    app.setApplicationName("Melampig");

    QDir dir(app.applicationDirPath() + "/../share/translations/ru");
    QString path = dir.absolutePath();

    QStringList list;
    list.append("melampig");
    list.append("ui");
    list.append("orm");
    list.append("qt");

    for ( int i = 0; i < list.size(); i++ ) {
        QTranslator *tr = new QTranslator();
        tr->load( list.at(i), path );
        app.installTranslator(tr);
    }

    int retval = 0;
	StartUpDialog s(0);
	if ( s.exec() == QDialog::Accepted )
	{
		MainWindow *mw = new MainWindow( s.getProject() );
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
