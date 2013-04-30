#include "reports.h"

#include "listreports.h"
#include "xmlreport.h"
#include "settingsform.h"

#include <ui.h>
#include <QApplication>
#include <QDebug>
#include <QDir>

namespace Melampig
{
    extern "C" PLUGIN_EXPORT ReportsPlugin *CreatePlugin() {
        return new ReportsPlugin();
    }

    ReportsPlugin::ReportsPlugin() : PluginBase()
    {
        QDir dir(qApp->applicationDirPath() + "/../share/translations/ru");
        QString path = dir.absolutePath();

        QTranslator *translator = new QTranslator();
        translator->load( "reports", path );
        qApp->installTranslator(translator);
    }

    ReportsPlugin::~ReportsPlugin()
    {
    }

    QString ReportsPlugin::name()
    {
        return "Reports";
    }

    QString ReportsPlugin::caption()
    {
        return tr("Reports");
    }

    QIcon ReportsPlugin::icon()
    {
        return QIcon(":/icons/report.png");
    }

    int ReportsPlugin::numCategories()
    {
        return 3;
    }

    QString ReportsPlugin::categoryName( int i )
    {
        QString name;
        switch( i )
        {
            case 0:	name = tr("Reports"); break;
            case 1:	name = tr("Summary XML Report Athlet for"); break;
            case 2:	name = tr("Settings"); break;
            default: name = tr("...missing name..."); break;
        }
        return name;
    }

    QIcon ReportsPlugin::categoryIcon( int i )
    {
        switch ( i )
        {
            case 0:  return QIcon( ":/icons/report.png");
            case 1:  return QIcon( ":/icons/report_user.png");
            case 2:  return QIcon( ":/settings-icon");
            default: return QIcon();
        }
    }

    QWidget *ReportsPlugin::setCurrentCategory( int i, Keeper *keeper, QWidget *parent )
    {
        QWidget *widget = 0;
        TQueryMap opt;

        switch( i )
        {
        case 0: {
            widget = new ListReports(keeper, parent);
            break;
        }
        case 1: {
            widget = new XmlReport(keeper, parent);
            break;
        }
        case 2: {
            widget = new SettingsForm(keeper, parent);
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

