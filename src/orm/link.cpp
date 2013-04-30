// link.cpp

#include "link.h"

namespace Melampig
{
	Link::Link(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Link::Link( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Link::Link ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Link::init()
	{
		table = QString("link");

		Object::setObjectName("Melampig::Link");
		type = OLink;
		mtype = aTable;

		//		name		name			Title			type		ro	visible
		schema.append( new Attr( "src",       tr("Source"),      Attr::ObjectRef, false, true ) );
		schema.append( new Attr( "dst",       tr("Destination"), Attr::ObjectRef, false, true ) );
	}

}
