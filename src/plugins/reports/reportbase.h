#ifndef MELAMPIG_REPORTBASE_H
#define MELAMPIG_REPORTBASE_H

#include <QMap>
#include <QString>
#include <QObject>

namespace Melampig
{
	class Keeper;
	class Object;

	class ReportBase : public QObject
	{
		Q_OBJECT
	public:
		ReportBase(Object *obj, Keeper *keeper, QObject *parent = 0);
		~ReportBase();

		virtual QString generate(bool) = 0;

	protected:
		void setReportPath(const QString &path) { reportPath = path; }
		void setReportFile(const QString &file) { reportFile = file; }


		QString loadTemplate(const QString &name);
		QString applyTemplateVars( const QString &html, const QMap<QString,QString> &param );

		Keeper *keeper;
		Object *comp;

		QString reportPath;
		QString reportFile;

		QMap<QString,QString> vars;
	};
}

#endif // REPORTBASE_H
