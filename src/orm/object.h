// object.h

#ifndef MELAMPIG_OBJECT_H
#define MELAMPIG_OBJECT_H

#include <QObject>
#include <QList>
#include <QHash>

#include "attr.h"
#include "keeper.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Object : public QObject
	{
		friend class Keeper;
		Q_OBJECT

		public:
			Object(Keeper *keeper);

			Object(const Object &obj);
			Object &operator=( const Object &obj );

			~Object();

			QList<Attr*> attrSchema() const { return schema; }

			QStringList nameSchema() const;
			Attr *attribute( const QString &name );

			QString getTable() const { return table; }
			QStringList getViewItem() const { return viewItems; }
			QStringList getSearchItems() const { return searchItems; }
			QStringList getFormItems() const { return formItems; }

			void store();
			void remove();

			QString get( const QString &key );
			void set( const QString &key, const QString &value );
			bool contains( const QString &key ) { return data.contains(key); }

			void restore( int id );
			void restore( QSqlQuery &query );
			void restore( const TQueryMap &opts );

			ModelType getModelType() { return mtype; }
			ObjectType getType() { return type; }
			bool isSearchAble() { return searchable; }

			Keeper *getKeeper() { return keeper; }

			static QStringList toList( const QString &str );
			static QString toArray( const QStringList &list );

		protected:
			void init();

			Keeper *keeper;

			QList<Attr*> schema;

			QStringList formItems;
			QStringList viewItems;
			QStringList searchItems;

			QHash< QString, QString > data;

			QString table;
			ObjectType type;

			int id;
			bool searchable;

			ModelType mtype;
	};
}

#endif // MELAMPIG_OBJECT_H
