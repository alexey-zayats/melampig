#include "competitionweight.h"

namespace Melampig
{
	CompetitionWeight::CompetitionWeight(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CompetitionWeight::CompetitionWeight( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CompetitionWeight::CompetitionWeight ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CompetitionWeight::init()
	{
		table = QString("competition_weight");
		type = OCompetitionWeight;
		mtype = aTable;

		Object::setObjectName("Melampig::CompetitionWeight");

		//		                Name     Title       ViewType            ro	visible
		schema.append( new Attr( "competition",      tr("Competition"),  Attr::ObjectRef,    false, true, OCompetition ) );
		schema.append( new Attr( "weight",      tr("Weight"),       Attr::ObjectRef,    false, true, OWeight) );

		viewItems.append( "competition" );
		viewItems.append( "weight" );
	}
}
