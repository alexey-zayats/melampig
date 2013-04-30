// keeper.h

#ifndef MELAMPIG_KEEPER_H
#define MELAMPIG_KEEPER_H

#define VERSION 0.0.1

#include <QObject>
#include <QHash>
#include <QList>
#include <QSqlDatabase>
#include <QStringList>

#include "errors.h"
#include "exports.h"
#include "defines.h"

namespace Melampig
{
	class Object;

	class MELAMPIG_ORM_EXPORT Keeper : QObject
	{
		Q_OBJECT

		public:
			// implicit constructor
			Keeper();
			virtual ~Keeper();

			// open connection to pgsql backend based on options map
			void open( const QHash<QString,QString> &opt );
			void reOpen();

			// close connection
			void close();

			// return object for sql queries
			QSqlDatabase database() const { return db; }
			QString getAlias() const { return alias; }
			QHash<QString,QString> getConnectOpt() const { return connectOpt; }

			// prepare params for query execution
			TQueryParam prepareParam( int flags, TQueryParam &values );
			// prepare params for query execution
			TQueryParam prepareParam( int flags, QVariant value );

			// parse query map and return QSqlQuery
			QSqlQuery parseQueryMap( const QString &view, const QStringList &nameSchema, const QStringList &wList, const TQueryMap &opts );

			QList<Object*> getObjectList( const ObjectType &type, const TQueryMap &opts );
			QList<QVariant> getFieldList( const ObjectType &type, const QString &filed, const TQueryMap &opts );
			QList<QVariant> getListOfFields( const ObjectType &type, const QStringList &fileds, const TQueryMap &opts );
			QHash<int,QVariant> getFieldMap( const ObjectType &type, const QString &filed, const TQueryMap &opts );
			QHash<QString,Object*> getObjectMap( const ObjectType &type, const QString &filed, const TQueryMap &opts );
			int countObjects( const ObjectType &type, const TQueryMap &opts );

			void clearObjectList( QList<Object*> list );
			Object *objectByType( const ObjectType &type );

		private:
			// parse one query item
			void parseQueryParam( const QString &name, const QVariant &params );

			QString tableByType( const ObjectType &type );
			QStringList filedsByType( const ObjectType &type );

			QList<QVariant> binds;
			QStringList wheres;
			QString groups,
					orders,
					offset,
					limit;

			QString alias;
			QString roleName, rolePass;
			QSqlDatabase db;
			QHash<QString,QString> connectOpt;
	};
}

#endif // MELAMPIG_KEEPER_H
