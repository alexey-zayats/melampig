#include "weight.h"

namespace Melampig
{
	Weight::Weight(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Weight::Weight( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Weight::Weight ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Weight::init()
	{
		table = QString("weight");
		type = OWeight;
		mtype = aList;

		Object::setObjectName("Melampig::Weight");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",   tr("title"),      Attr::Int,         false, true ) );

		viewItems.append( "title" );
	}

}
