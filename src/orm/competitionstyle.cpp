#include "competitionstyle.h"

namespace Melampig
{
	CompetitionStyle::CompetitionStyle(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CompetitionStyle::CompetitionStyle( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CompetitionStyle::CompetitionStyle ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CompetitionStyle::init()
	{
		table = QString("competition_style");
		type = OCompetitionStyle;
		mtype = aTable;

		Object::setObjectName("Melampig::CompetitionStyle");

		//		                Name     Title       ViewType            ro	visible
		schema.append( new Attr( "competition",      tr("Competition"),  Attr::ObjectRef,    false, true, OCompetition ) );
		schema.append( new Attr( "style",      tr("Style"),        Attr::ObjectRef,    false, true, OStyle ) );

		viewItems.append( "competition" );
		viewItems.append( "style" );
	}
}
