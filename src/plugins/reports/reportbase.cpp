#include "reportbase.h"

#include <QDir>
#include <QApplication>
#include <QFile>
#include <QDebug>

#include <orm.h>

namespace Melampig
{
	ReportBase::ReportBase(Object *obj, Keeper *keeper, QObject *parent)
		: QObject(parent),
		  keeper(keeper),
		  comp(obj)
	{
	}

	ReportBase::~ReportBase()
	{}

	QString ReportBase::loadTemplate(const QString &name)
	{
		QString filePath = QDir::cleanPath(QApplication::applicationDirPath() + reportPath) + QString("/") + name;

		QFile file( filePath );

		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return QString("Template \"%1\" is not found").arg(name);

		QString html;
		while (!file.atEnd()) {
			html += file.readLine();
		}

		file.close();
		return html;
	}

	QString ReportBase::applyTemplateVars( const QString &html, const QMap<QString,QString> &param )
	{
		QString data = html;
		QMap<QString,QString>::const_iterator it =  param.begin();
		for( ; it != param.end(); it++ ) {
			data = data.replace(it.key(), it.value());
		}
		return data;
	}
}
