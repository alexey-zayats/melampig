#ifndef MELAMPIG_REPORTSPLUGIN_H
#define MELAMPIG_REPORTSPLUGIN_H

#include <pluginbase.h>

namespace Melampig
{
	class ReportsPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		ReportsPlugin();
		~ReportsPlugin();

		QString name();
		QString caption();
		QIcon icon();

		int numCategories();
		QString categoryName( int );
		QIcon categoryIcon( int );

		QWidget *setCurrentCategory( int i, Keeper *keeper,  QWidget * );
	};
}

#endif // MELAMPIG_REPORTSPLUGIN_H

