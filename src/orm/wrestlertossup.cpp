#include "wrestlertossup.h"

namespace Melampig
{
	WrestlerTossup::WrestlerTossup(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	WrestlerTossup::WrestlerTossup( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	WrestlerTossup::WrestlerTossup ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void WrestlerTossup::init()
	{
		table = QString("team_tossup");
		type  = OWrestlerTossup;
		mtype = aTable;

		Object::setObjectName("Melampig::WrestlerTossup");

		//		                Name         Title              ViewType            ro	visible
		schema.append( new Attr( "wrestler", tr("Wrestler"),    Attr::ObjectRef,    false, true, OTeamWrestler ) );
		schema.append( new Attr( "sorder",   tr("Sort order"),  Attr::Int,          false, true ) );

		viewItems.clear();
		viewItems.append( "wrestler" );
		viewItems.append( "sorder" );
	}
}
