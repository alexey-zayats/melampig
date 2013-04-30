#include "team.h"

namespace Melampig
{

	Team::Team(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Team::Team( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Team::Team ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Team::init()
	{
		table = QString("team");
		type = OTeam;
		mtype = aTable;

		Object::setObjectName("Melampig::Team");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),         Attr::String,   false, true ) );
		schema.append( new Attr( "geo",            tr("Geo"),           Attr::ObjectRef,    false, true, OGeo ) );
		schema.append( new Attr( "style",          tr("Style"),         Attr::ObjectRef,    false, true, OStyle ) );

		viewItems.append( "title" );
		viewItems.append( "geo" );
		viewItems.append( "style" );
	}

}
