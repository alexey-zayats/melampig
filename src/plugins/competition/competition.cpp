#include "competition.h"
#include "complist.h"
#include <QApplication>
#include <QDebug>
#include <QDir>

namespace Melampig
{
	extern "C" PLUGIN_EXPORT CompetitionPlugin *CreatePlugin() {
		return new CompetitionPlugin();
	}

    CompetitionPlugin::CompetitionPlugin() : PluginBase()
    {
        QDir dir(qApp->applicationDirPath() + "/../share/translations/ru");
        QString path = dir.absolutePath();

        QTranslator *translator = new QTranslator();
        translator->load( "competition", path );
        qApp->installTranslator(translator);
    }

    CompetitionPlugin::~CompetitionPlugin()
    {
    }

    QString CompetitionPlugin::name()
    {
        return "CompetitionPlugin";
    }

    QString CompetitionPlugin::caption()
    {
        return tr("Competition");
    }

    QIcon CompetitionPlugin::icon()
    {
        return QIcon(":/flag-red");
    }

    int CompetitionPlugin::numCategories()
    {
        return 1;
    }

    QString CompetitionPlugin::categoryName( int i )
    {
        QString name;
        switch( i )
        {
        case 0:	name = tr("Competition"); break;
        default: name = tr("...missing name..."); break;
        }
        return name;
    }

    QIcon CompetitionPlugin::categoryIcon( int i )
    {
        switch ( i )
        {
        case 0:	return QIcon(":/flag-red");
        default: return QIcon(":/icons/cog.png");
        }
    }

    QWidget *CompetitionPlugin::setCurrentCategory( int i, Keeper *keeper, QWidget *parent )
    {
        QWidget *widget = 0;
        TQueryMap opt;

        switch( i )
        {
            case 0:
            {
                widget = new CompList(opt, keeper, parent);
                break;
            }
            default:
            {
                widget = new QWidget( parent );
                QLabel *label = new QLabel(widget);
                label->setText("Not implemented yet(?)");
                label->setAlignment(Qt::AlignCenter);
                break;
            }
        }
        return widget;
    }
}

