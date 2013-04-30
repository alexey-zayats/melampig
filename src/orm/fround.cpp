#include "fround.h"

namespace Melampig
{
	FRound::FRound(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	FRound::FRound( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	FRound::FRound ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void FRound::init()
	{
		table = QString("fround");
		type = OFRound;
		mtype = aList;

		Object::setObjectName("Melampig::FRound");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",   tr("Title"),     Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
