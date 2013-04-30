#include "geo.h"

namespace Melampig
{
	Geo::Geo(Keeper *keeper) : Tree(keeper)
	{
		init();
	}

	Geo::Geo( int id, Keeper *keeper) : Tree(keeper)
	{
		init();
		restore( id );
	}

	Geo::Geo ( const TQueryMap &opts, Keeper *keeper) : Tree(keeper)
	{
		init();
		restore( opts );
	}

	void Geo::init()
	{
		table = QString("geo");
		type = OGeo;
		mtype = aTree;

		Object::setObjectName("Melampig::Geo");

		//		Name		Name			ViewType	ro	visible
		schema.append( new Attr( "title",   tr("Title"),    Attr::String,   false, true ) );
		schema.append( new Attr( "num",     tr("Numeric"),  Attr::String,   false, true ) );
		schema.append( new Attr( "alpha2",  tr("Alpha2"),   Attr::String,   false, true ) );
		schema.append( new Attr( "alpha3",  tr("Alpha3"),   Attr::String,   false, true ) );
		schema.append( new Attr( "flag",    tr("Flag"),     Attr::Svg,    false, true ) );

		attribute("pid")->setRef( OGeo );

		viewItems.append( "title" );
	}
}
