// blob.cpp

#include "blob.h"
#include "qsql_psql.h"
#include <QSqlQuery>
#include <QSqlError>

namespace CMS
{
	Blob::Blob ( Keeper *keeper ) : Object ( keeper )
	{
		init();
	}

	Blob::Blob ( Keeper *keeper, int idx ) : Object ( keeper )
	{
		init();
		restore( idx );
	}

	Blob::Blob ( Keeper *keeper, const TQueryMap &opts ) : Object ( keeper )
	{
		init();
		restore( opts );
	}

	void Blob::init()
	{
		table = QString("blob");

		Object::setObjectName("CMS::Blob");
		type = OBlob;

		// overloading :)
// 		schema.remove( "name" );
		//schema.insert( "name", new Attr( "name", tr("Filename"), Attr::File, false, true ) );
		
/*		
		Attr *a = 0;
		TSchema::iterator it = schema.begin();
		for( ; it != schema.end(); it++ ) {
			if ( it.value()->getName().compare("acl") == 0  ||
						  it.value()->getName().compare("usr") == 0 ||
						  it.value()->getName().compare("grp") == 0 ||
						  it.value()->getName().compare("param") == 0
			   ) {
				a = schema.take( it.key() );
				delete a;
			   }
		}
		
		viewItems.removeAll( "acl" );
		viewItems.removeAll( "usr" );
		viewItems.removeAll( "grp" );
*/

		//             name		name			Title			ViewType	ro	visible
		schema.insert( "pid",		new Attr( "pid",		tr("Parent object"),	Attr::Object,	false, 	true ) );
		schema.insert( "score",		new Attr( "score",		tr("Score"),		Attr::Int,	false, 	true ) );
		schema.insert( "lo",		new Attr( "lo",		    tr("Large object"),	Attr::ByteArray,	false, 	true ) );
		schema.insert( "title",		new Attr( "title",		tr("Title"),		Attr::String,	false, 	true ) );
		schema.insert( "description",	new Attr( "description",	tr("Description"),	Attr::Text, 	false, 	true ) );
		schema.insert( "mimetype",	new Attr( "mimetype",	tr("Mime type"),	Attr::String,	false, 	true ) );
		schema.insert( "size",		new Attr( "size",		tr("Size"),		Attr::Int,	false, 	true ) );
		schema.insert( "filename", new Attr( "filename",    tr("Filename"), Attr::File, false, true ) );

		viewItems.append( "pid" );
		viewItems.append( "title" );
		viewItems.append( "filename" );
		viewItems.append( "size" );
		viewItems.append( "mimetype" );
		viewItems.append( "score" );
		viewItems.append( "description" );

		searchable = true;
		searchItems.append("title");
		searchItems.append("description");
	}

	bool Blob::writeLO( const QByteArray *ba )
	{
		if ( !keeper->database().isOpen() ) 
			keeper->reOpen();
		
		QSqlQuery query( keeper->database() );
		query.prepare( "UPDATE blob SET lo = ?, size = ? WHERE id = ?");
		query.addBindValue( *ba );
		query.addBindValue( ba->size() );
		query.addBindValue( get("id") );
		if ( !query.exec() )
			throw SqlError( query.lastError().text(), query.executedQuery() );

		return true;
		
// 		int lo = get( "lo" ).toInt();
// 
// 		QPSQLDriver *drv = (QPSQLDriver*)keeper->database().driver();
// 		if ( !drv ) return false;
// 
// 		if ( lo )
// 		{
// 			drv->loUnlink( lo );
// 			set( "lo", 0 );
// 			lo = 0;
// 		}
// 
// 		lo = drv->loCreate();
// 		if ( !lo ) return false;
// 
// 		if ( drv->loWrite( lo, ba ) )
// 		{
// 			set( "lo", QString::number(lo) );
// 			set( "size", QString::number( ba.size() ) );
// 			return true;
// 		}

		//return false;
	}

	QByteArray Blob::readLO()
	{
		if ( !keeper->database().isOpen() ) 
			keeper->reOpen();
		
		QSqlQuery query( keeper->database() );
		query.prepare( "SELECT lo FROM blob WHERE id = ?");
		query.addBindValue( get("id") );
		query.exec();

		query.next();
		
// 		QByteArray ba;
// 		int lo = get( "lo" ).toInt();
// 		if ( !lo ) return ba;
// 
// 		QPSQLDriver *drv = (QPSQLDriver*)keeper->database().driver();
// 		if ( !drv ) return ba;
// 
// 		ba = drv->loRead( lo );
		return query.value(0).toByteArray();
	}

}

