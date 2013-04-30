#include "circle.h"

namespace Melampig
{
	Circle::Circle(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Circle::Circle( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Circle::Circle ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Circle::init()
	{
		table = QString("circle");
		type = OCircle;
		mtype = aList;

		Object::setObjectName("Melampig::Circle");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
