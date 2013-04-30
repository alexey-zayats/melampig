#include "competitionreferee.h"

namespace Melampig
{
	CompetitionReferee::CompetitionReferee(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CompetitionReferee::CompetitionReferee( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CompetitionReferee::CompetitionReferee ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CompetitionReferee::init()
	{
		table = QString("competition_referee");
		type = OCompetitionReferee;
		mtype = aTable;

		Object::setObjectName("Melampig::CompetitionReferee");

		//		                Name     Title       ViewType            ro	visible
		schema.append( new Attr( "competition",      tr("Competition"),  Attr::ObjectRef,    false, true, OCompetition ) );
		schema.append( new Attr( "referee",      tr("Referee"),      Attr::ObjectRef,    false, true, OReferee ) );
		schema.append( new Attr( "position", tr("Position"),     Attr::ObjectRef,    false, true, OPosition ) );

		viewItems.append( "competition" );
		viewItems.append( "referee" );
		viewItems.append( "position" );
	}
}
