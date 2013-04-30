#include "tour.h"

namespace Melampig
{

	Tour::Tour(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Tour::Tour( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Tour::Tour ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Tour::init()
	{
		table = QString("tour");
		type = OTour;
		mtype = aList;

		Object::setObjectName("Melampig::Tour");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.clear();
		viewItems.append( "title" );
	}
}
