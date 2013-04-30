#include "reference.h"
#include "referencestack.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

namespace Melampig
{
	extern "C" PLUGIN_EXPORT ReferencePlugin *CreatePlugin() {
		return new ReferencePlugin();
	}

	ReferencePlugin::ReferencePlugin() : PluginBase()
	{
		QDir dir(qApp->applicationDirPath() + "/../share/translations/ru");
		QString path = dir.absolutePath();

		QTranslator *translator = new QTranslator();
		translator->load( "reference", path );
		qApp->installTranslator(translator);
	}

	ReferencePlugin::~ReferencePlugin()
	{
	}

	QString ReferencePlugin::name()
	{
		return "Reference";
	}

	QString ReferencePlugin::caption()
	{
		return tr("Reference");
	}

	QIcon ReferencePlugin::icon()
	{
		return QIcon(":/share/icons/copy.png");
	}

	int ReferencePlugin::numCategories()
	{
		return 1;
	}

	QString ReferencePlugin::categoryName( int i )
	{
		QString name;
		switch( i )
		{
			case 0:	name = tr("Reference"); break;
			default: name = tr("...missing name..."); break;
		}
		return name;
	}

	QIcon ReferencePlugin::categoryIcon( int i )
	{
		switch ( i )
		{
			case 0:  return QIcon( ":/icons/chart_organisation.png");
			default: return QIcon();
		}
	}

	QWidget *ReferencePlugin::setCurrentCategory( int i, Keeper *keeper, QWidget *parent )
	{
		QLabel *label = 0;
		QWidget *widget = 0;

		switch ( i )
		{
			case 0:  {
				widget = new ReferenceStack(keeper, parent);
				break;
			}
			default: {
				widget = new QWidget( parent );
				label = new QLabel(widget);
				label->setText("Not implemented. Yet?");
				label->setAlignment(Qt::AlignCenter);
				widget->setMaximumSize(100, 100);
			}
		}

		return widget;
	}

}

