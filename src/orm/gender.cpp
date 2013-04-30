#include "gender.h"

namespace Melampig
{

	Gender::Gender(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Gender::Gender( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Gender::Gender ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Gender::init()
	{
		table = QString("gender");
		type = OGender;
		mtype = aList;

		Object::setObjectName("Melampig::Gender");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
