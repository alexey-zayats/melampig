#include "comptype.h"

namespace Melampig
{
	CompType::CompType(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CompType::CompType( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CompType::CompType ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CompType::init()
	{
		table = QString("comptype");
		type = OCompType;
		mtype = aList;

		Object::setObjectName("Melampig::CompType");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
