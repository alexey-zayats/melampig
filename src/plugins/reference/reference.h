#ifndef MELAMPIG_REFERENCEPLUGIN_H
#define MELAMPIG_REFERENCEPLUGIN_H

#include <pluginbase.h>

namespace Melampig
{
	class ReferencePlugin : public PluginBase
	{
		Q_OBJECT
	public:
		ReferencePlugin();
		~ReferencePlugin();

		QString name();
		QString caption();
		QIcon icon();

		int numCategories();
		QString categoryName( int );
		QIcon categoryIcon( int );

		QWidget *setCurrentCategory( int i, Keeper *keeper,  QWidget * );
	};
}

#endif // MELAMPIG_REFERENCEPLUGIN_H

