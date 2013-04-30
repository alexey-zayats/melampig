#include "position.h"

namespace Melampig
{
	Position::Position(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Position::Position( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Position::Position ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Position::init()
	{
		table = QString("position");
		type = OPosition;
		mtype = aList;

		Object::setObjectName("Melampig::Position");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
