#include "competitionwrestler.h"

namespace Melampig
{
	CompetitionWrestler::CompetitionWrestler(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CompetitionWrestler::CompetitionWrestler( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CompetitionWrestler::CompetitionWrestler ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CompetitionWrestler::init()
	{
		table = QString("competition_wrestler");
		type = OCompetitionWrestler;
		mtype = aTable;

		Object::setObjectName("Melampig::CompetitionWrestler");

		//		                Name        Title              ViewType            ro	visible
		schema.append( new Attr( "competition",   tr("Competition"),  Attr::ObjectRef,    false, true, OCompetition ) );
		schema.append( new Attr( "wrestler",      tr("Wrestler"),     Attr::ObjectRef,    false, true, OWrestler ) );
		schema.append( new Attr( "team",          tr("Team"),         Attr::ObjectRef,    false, true, OTeam ) );
		schema.append( new Attr( "style",         tr("Style"),        Attr::ObjectRef,    false, true, OStyle ) );
		schema.append( new Attr( "weight",        tr("Weight"),       Attr::ObjectRef,    false, true, OWeight ) );
		schema.append( new Attr( "heft",          tr("Heft"),         Attr::Double,          false, true ) );
		schema.append( new Attr( "sorder",        tr("Order"),        Attr::Int,          false, true ) );

		viewItems.append( "competition" );
		viewItems.append( "team" );
		viewItems.append( "wrestler" );
		viewItems.append( "style" );
		viewItems.append( "weight" );
		viewItems.append( "heft" );
	}
}
