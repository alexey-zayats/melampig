#include "role.h"

namespace Melampig
{
	Role::Role(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Role::Role( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Role::Role ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Role::init()
	{
		table = QString("role");
		type = ORole;
		mtype = aList;

		Object::setObjectName("Melampig::Role");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
