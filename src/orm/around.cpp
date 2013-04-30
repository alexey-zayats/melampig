#include "around.h"

namespace Melampig
{
	ARound::ARound(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	ARound::ARound( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	ARound::ARound ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void ARound::init()
	{
		table = QString("around");
		type = OARound;
		mtype = aList;

		Object::setObjectName("Melampig::OARound");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",   tr("Title"),     Attr::String,   false, true ) );

		viewItems.clear();
		viewItems.append( "title" );
	}
}
