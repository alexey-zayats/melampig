#include "age.h"

namespace Melampig
{
	Age::Age(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Age::Age( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Age::Age ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Age::init()
	{
		table = QString("age");
		type = OAge;
		mtype = aList;

		Object::setObjectName("Melampig::Age");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
