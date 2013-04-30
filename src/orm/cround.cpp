#include "cround.h"

namespace Melampig
{

	CRound::CRound(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CRound::CRound( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CRound::CRound ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CRound::init()
	{
		table = QString("cround");
		type = OCRound;
		mtype = aList;

		Object::setObjectName("Melampig::CRound");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title", tr("Title"),   Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
