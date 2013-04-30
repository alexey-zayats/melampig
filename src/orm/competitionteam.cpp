#include "competitionteam.h"

namespace Melampig
{
	CompetitionTeam::CompetitionTeam(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CompetitionTeam::CompetitionTeam( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CompetitionTeam::CompetitionTeam ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CompetitionTeam::init()
	{
		table = QString("competition_team");
		type = OCompetitionTeam;
		mtype = aTable;

		Object::setObjectName("Melampig::CompetitionTeam");

		//		                Name     Title       ViewType            ro	visible
		schema.append( new Attr( "competition",      tr("Competition"),  Attr::ObjectRef,    false, true, OCompetition ) );
		schema.append( new Attr( "team",      tr("Team"),         Attr::ObjectRef,    false, true, OTeam ) );
		schema.append( new Attr( "cgroup",      tr("Group"),         Attr::ObjectRef,    false, true, OCGroup ) );
		schema.append( new Attr( "style",      tr("Style"),         Attr::ObjectRef,    false, true, OStyle ) );
		schema.append( new Attr( "sorder",      tr("Order"),         Attr::Int,    false, true) );

		viewItems.append( "competition" );
		viewItems.append( "team" );
		viewItems.append( "cgroup" );
		viewItems.append( "style" );
	}
}
