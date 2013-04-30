#include "mainwindow-s.h"

#include <QApplication>
#include <QSettings>
#include <QTextCodec>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QTextCodec *codec=QTextCodec::codecForName("UTF8");
	QTextCodec::setCodecForCStrings(codec);

    Q_INIT_RESOURCE(melampig);

	QApplication app(argc, argv);
	app.setOrganizationName("Agitum");
	app.setOrganizationDomain("agitum.ru");
    app.setApplicationName("Scoreboard");

    QTranslator *translator = new QTranslator();
    translator->load( "scoreboard_ru", app.applicationDirPath() + "/../share/translations" );
    app.installTranslator(translator);

    translator = new QTranslator();
    translator->load( "qt_ru", app.applicationDirPath() + "/../share/translations" );
    app.installTranslator(translator);

    MainWindow *w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    QObject::connect(w, SIGNAL(destroyed()), &app, SLOT(quit()));

    return app.exec();
}
