#include "ctour.h"

namespace Melampig
{

	CTour::CTour(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CTour::CTour( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CTour::CTour ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CTour::init()
	{
		table = QString("ctour");
		type = OCTour;
		mtype = aTable;

		Object::setObjectName("Melampig::CTour");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title", tr("Title"),   Attr::String,   false, true ) );
		schema.append( new Attr( "num",   tr("Num"),     Attr::Int,   false, true ) );

		viewItems.append( "title" );
		viewItems.append( "num" );
	}
}
