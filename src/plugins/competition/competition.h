#ifndef MELAMPIG_Competition_H
#define MELAMPIG_Competition_H

#include <pluginbase.h>

namespace Melampig
{
	class CompetitionPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		CompetitionPlugin();
		~CompetitionPlugin();

		QString name();
		QString caption();
		QIcon icon();

		int numCategories();
		QString categoryName( int );
		QIcon categoryIcon( int );

		QWidget *setCurrentCategory( int i, Keeper *keeper, QWidget * );
	};
}

#endif // MELAMPIG_Competition_H

