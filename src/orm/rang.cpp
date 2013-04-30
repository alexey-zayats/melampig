#include "rang.h"

namespace Melampig
{

	Rang::Rang(Keeper *keeper) : Tree(keeper)
	{
		init();
	}

	Rang::Rang( int id, Keeper *keeper) : Tree(keeper)
	{
		init();
		restore( id );
	}

	Rang::Rang ( const TQueryMap &opts, Keeper *keeper) : Tree(keeper)
	{
		init();
		restore( opts );
	}

	void Rang::init()
	{
		table = QString("rang");
		type = ORang;
		mtype = aTree;

		Object::setObjectName("Melampig::Rang");

							   // Name       Title     ViewType        ro     visible
		schema.append( new Attr( "title", tr("Title"), Attr::String,   false, true ) );
		schema.append( new Attr( "acr",   tr("Acr"),   Attr::String,   false, true ) );

		viewItems.append( "title" );
		viewItems.append( "acr" );
	}
}
