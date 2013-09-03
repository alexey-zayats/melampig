// object.cpp

#include "object.h"

#include <QFile>
#include <QTextStream>

#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextCodec>
#include <QDebug>

namespace Melampig
{

	Object::Object(Keeper *keeper) : QObject(), keeper(keeper)
	{
			init();
	}

	Object::Object( const Object &obj ) : QObject()
	{
		this->keeper = obj.keeper;
		this->schema = obj.schema;
		this->formItems = obj.formItems;
		this->viewItems = obj.viewItems;
		this->searchItems = obj.searchItems;
		this->data = obj.data;
		this->table = obj.table;
		this->type = obj.type;
		this->id = obj.id;
		this->searchable = obj.searchable;
	}

	Object &Object::operator=( const Object &obj )
	{
		this->keeper = obj.keeper;
		this->schema = obj.schema;
		this->formItems = obj.formItems;
		this->viewItems = obj.viewItems;
		this->searchItems = obj.searchItems;
		this->data = obj.data;
		this->table = obj.table;
		this->type = obj.type;
		this->id = obj.id;
		this->searchable = obj.searchable;
		return *this;
	}

	Object::~Object()
	{
		qDeleteAll(schema);
	}

	Attr *Object::attribute( const QString &name )
	{
		Attr *a = 0;

		for( int i = 0; i < schema.size(); i++ ) {
			a =  schema.at(i);
			if ( a->getName().compare(name) == 0 )
				return a;
			else
				a = 0;
		}
		return a;
	}

	void Object::init()
	{
//		qDebug() << Q_FUNC_INFO;
		//                  name               name         Title                type            ro	    visible
		schema.append( new Attr( "id",      tr("ID"),               Attr::Int,      true,  true ) );
        schema.append( new Attr( "guid",    tr("GUID"),             Attr::String,   true,  true ) );
		schema.append( new Attr( "relname", tr("RelName"),          Attr::String,   false, true ) );
		schema.append( new Attr( "name",    tr("Name"),             Attr::String,   false, true ) );
		schema.append( new Attr( "created", tr("Create date/time"), Attr::DateTime, true,  true ) );
		schema.append( new Attr( "updated", tr("Update date/time"), Attr::DateTime, true,  true ) );

		id = 0;
		type = OObject;
		mtype = aList;
		table = QString("obj");

		Object::setObjectName("Melampig::Object");

//        viewItems.append( "id" );
//        viewItems.append( "name" );
//        viewItems.append( "created" );
	}

	QStringList Object::nameSchema() const
	{
		QStringList names;
		for( int i = 0; i < schema.size(); i++ ) {
			names.append(  schema.at(i)->getName() );
		}

		return names;
	}

	QString Object::get( const QString &key )
	{
		QString out;

		if ( attribute(key) != 0 && data.contains(key) )
			out = data[key];

		return out;
	}

	void Object::set( const QString &key, const QString &value )
	{
		if ( attribute(key) == 0 ) {
//			qDebug() << Q_FUNC_INFO << key << "attribute not exists";
			return;
		}
		if ( data.contains( key ) ) data.remove( key );
		data.insert( key, value );
		if ( key.compare("id") == 0 ) id = value.toInt();
	}

	void Object::store()
	{
		if ( table.isEmpty() )
			return;

		QStringList field;
		QStringList ph;
		QList<QString> binds;

		QString val;


		Attr *a = 0;
		QHashIterator< QString, QString > i(data);
		while (i.hasNext() )
		{
			i.next();

			// we reject not schemed pairs key=>value
			a = attribute(i.key() );
			if ( a == 0 )
				continue;

			if ( id && a->isReadOnly() )
				continue;

            //if ( !a->isVisible() )
            //	continue;

			val = i.value();

			if ( id  )
			{
				// for update
				if ( i.key().compare("id") )
				{
					if ( a->getRef() != ONull && a->isArray() == false && val.toInt() == 0  )
					{
						field.append( QString( "%1 = NULL" ).arg( i.key() ) );
					} else if ( a->getType() == Attr::Svg || a->getType() == Attr::Image ) {
						field.append( QString( "%1 = decode(?, 'base64')" ).arg( i.key() ) );
						binds.append( val );
					} else {
						field.append( QString( "%1 = ?" ).arg( i.key() ) );
						binds.append( val );
					}
				}
			}
			else // for insert
			{
				field.append( i.key() );

				if ( a->getRef() != ONull && val.toInt() == 0  ) {
					ph.append( "NULL" );
				} else if ( a->getType() == Attr::Svg || a->getType() == Attr::Image ) {
					ph.append( "decode(?, 'base64')" );
					binds.append( val );
				} else {
					ph.append( "?" );
					binds.append( val );
				}
			}
		}

		QString sql = id ?
					QString( "UPDATE %1 SET %2 WHERE id = ?" ).arg( table ).arg( field.join(", ") ) :
					QString( "INSERT INTO %1 (%2) VALUES (%3)" ).arg( table ).arg( field.join(", ") ).arg( ph.join(", ") ) ;

		QSqlQuery query( keeper->database() );
		query.prepare( sql );

		for( int i = 0; i < binds.size(); i++ )
			query.addBindValue( binds.at(i) );

		if ( id ) query.addBindValue( id );

		if ( !query.exec() ) {
			throw SqlError( query.lastError().text(), query.executedQuery() );
		}

        qDebug() << query.executedQuery() << binds;

		if ( !id ) {
			query.exec( "SELECT currval('obj_id_seq')" );
			query.next();
			set( "id", query.value(0).toString() );
		}

//		restore( get("id").toInt() );
	}

	void Object::remove()
	{
		if ( table.isEmpty() || id < 0 )
			return;

		QString sql = QString( "DELETE FROM %1 WHERE id = ?" ).arg( table );

		QSqlQuery query( keeper->database() );
		query.prepare( sql );
		query.addBindValue( id );

        qDebug() << query.executedQuery() << id;

		if ( !query.exec() )
			throw SqlError( query.lastError().text(), query.executedQuery() );
	}

	void Object::restore( const TQueryMap &opts )
	{
		QStringList flist = nameSchema();
		QSqlQuery query = keeper->parseQueryMap( table, flist, flist, opts );

		if ( !query.exec() )
			throw SqlError( query.lastError().text(), query.executedQuery() );

		if ( !query.isActive() )
			return;

		if ( query.next() )
			restore( query );
	}

	void Object::restore( int idx )
	{
		TQueryMap opts;
		opts.insert( "id", keeper->prepareParam( Equal, idx ) );
		restore( opts );
	}

	void Object::restore( QSqlQuery &query )
	{
		Attr *a = 0;
		QStringList field = nameSchema();
		for( int i = 0; i < field.size(); i++ ) {
			a = attribute(field.at(i));
			switch ( a->getType() )
			{
				case Attr::Svg:
				case Attr::Image: {
					set( field.at(i), query.value(i).toByteArray().toBase64() );
					break;
			   }
			   default:
					set( field.at(i), query.value(i).toString() );
					break;
			}
		}
	}

	QStringList Object::toList( const QString &str )
	{
		QString a = str;
		a.remove( "{" );
		a.remove( "}" );
		return a.isEmpty() ? QStringList() : a.split( "," );
	}

	QString Object::toArray( const QStringList &list )
	{
		return "{" + list.join( "," ) + "}";
	}
}

