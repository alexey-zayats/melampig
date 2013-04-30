// pluginbase.h

#ifndef MELAMPIG_PLUGINBASE_H
#define MELAMPIG_PLUGINBASE_H

#include <QtGui>
#include "orm.h"

#ifdef Q_OS_WIN32
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT
#endif

namespace Melampig
{
	class PluginBase : public QObject
	{
		Q_OBJECT
	public:
		virtual QString name() = 0;
		virtual QString caption() = 0;
		virtual QIcon icon() = 0;
		virtual int numCategories() = 0;
		virtual QString categoryName( int i ) = 0;
		virtual QIcon categoryIcon( int i ) = 0;

	public slots:
		virtual QWidget *setCurrentCategory( int i, Keeper *keeper, QWidget *parent ) = 0;
	};
}

#endif // MELAMPIG_PLUGINBASE_H




